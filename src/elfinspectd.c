#include "argumentsd.h"
#include "contextd.h"
#include "elf32_header.h"
#include "elf64_header.h"
#include "elf_ident.h"
#include "elf_validator.h"
#include "errorsd.h"
#include "utild.h"
#include <ctype.h>
#include <p101_c/p101_stdlib.h>
#include <p101_c/p101_string.h>
#include <p101_convert/integer.h>
#include <p101_fsm/fsm.h>
#include <p101_posix/p101_string.h>
#include <p101_posix/p101_unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

enum states
{
    PARSE_ARGS = P101_FSM_USER_START,
    HANDLE_ARGS,
    USAGE,
    WAIT_FOR_REQUEST,
    PARSE_REQUEST,
    VERIFY_ELF_HEADER,
    RESPOND,
    CLEANUP_RESPONSE,
    CLEANUP_PROGRAM,
};

static volatile sig_atomic_t exit_flag    = 0;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
static volatile sig_atomic_t socket_close = 0;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

static void             setup_signal_handlers(void);
static void             sig_handler(int signal);
static p101_fsm_state_t parse_arguments(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t handle_arguments(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t wait_for_request(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t parse_request(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t verify_elf_header(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t respond(const struct p101_env *env, struct p101_error *err, void *ctx);
void                    free_if_not_null(const struct p101_env *env, char **buf);
static p101_fsm_state_t cleanup_response(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t usage(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t cleanup_program(const struct p101_env *env, struct p101_error *err, void *ctx);

#define ERR_MSG_LEN 256             // NOLINT(cppcoreguidelines-macro-to-enum, modernize-macro-to-enum)
#define RESPONSE_MSG_LEN 1024       // NOLINT(cppcoreguidelines-macro-to-enum, modernize-macro-to-enum)
#define SOCK_QUEUE 5                // NOLINT(cppcoreguidelines-macro-to-enum, modernize-macro-to-enum)
#define MAX_FILE_NAME_LEN 256       // NOLINT(cppcoreguidelines-macro-to-enum, modernize-macro-to-enum)
#define MAX_FILE_LEN 1048576        // NOLINT(cppcoreguidelines-macro-to-enum, modernize-macro-to-enum)
#define CLASS_LOCATION 4            // NOLINT(cppcoreguidelines-macro-to-enum, modernize-macro-to-enum)
#define MAX_ELF_ADDRESS_CHARS 19    // NOLINT(cppcoreguidelines-macro-to-enum, modernize-macro-to-enum)

static void setup_signal_handlers(void)
{
    struct sigaction action;

    memset(&action, 0, sizeof(struct sigaction));

#ifdef __clang__
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    action.sa_handler = sig_handler;
#ifdef __clang__
    #pragma clang diagnostic pop
#endif

    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if(sigaction(SIGINT, &action, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void sig_handler(int signal)
{
    if(signal == SIGINT)
    {
        exit_flag = 1;
    }
    else if(signal == SIGPIPE)
    {
        socket_close = 1;
    }
}

#pragma GCC diagnostic pop

int main(int argc, char *argv[])
{
    static struct p101_fsm_transition transitions[] = {
        {P101_FSM_INIT,     PARSE_ARGS,        parse_arguments  },
        {PARSE_ARGS,        USAGE,             usage            },
        {PARSE_ARGS,        HANDLE_ARGS,       handle_arguments },
        {HANDLE_ARGS,       USAGE,             usage            },
        {HANDLE_ARGS,       CLEANUP_PROGRAM,   cleanup_program  },
        {HANDLE_ARGS,       WAIT_FOR_REQUEST,  wait_for_request },
        {USAGE,             CLEANUP_PROGRAM,   cleanup_program  },
        {WAIT_FOR_REQUEST,  CLEANUP_PROGRAM,   cleanup_program  },
        {WAIT_FOR_REQUEST,  PARSE_REQUEST,     parse_request    },
        {PARSE_REQUEST,     CLEANUP_PROGRAM,   cleanup_program  },
        {PARSE_REQUEST,     RESPOND,           respond          },
        {PARSE_REQUEST,     VERIFY_ELF_HEADER, verify_elf_header},
        {VERIFY_ELF_HEADER, RESPOND,           verify_elf_header},
        {RESPOND,           CLEANUP_RESPONSE,  cleanup_response },
        {CLEANUP_RESPONSE,  WAIT_FOR_REQUEST,  wait_for_request },
        {CLEANUP_RESPONSE,  CLEANUP_PROGRAM,   cleanup_program  },
        {CLEANUP_PROGRAM,   P101_FSM_EXIT,     NULL             }
    };

    struct p101_error    *err;
    struct p101_env      *env;
    struct p101_fsm_info *fsm;
    p101_fsm_state_t      from_state;
    p101_fsm_state_t      to_state;
    struct p101_error    *fsm_err;
    struct p101_env      *fsm_env;
    struct argumentsd     args;
    struct contextd       ctx;

    setup_signal_handlers();

    err = p101_error_create(false);

    if(err == NULL)
    {
        ctx.exit_code = EXIT_FAILURE;
        goto done;
    }

    env = p101_env_create(err, true, NULL);

    if(p101_error_has_error(err))
    {
        ctx.exit_code = EXIT_FAILURE;
        goto free_error;
    }

    fsm_err = p101_error_create(false);

    if(fsm_err == NULL)
    {
        ctx.exit_code = EXIT_FAILURE;
        goto free_env;
    }

    fsm_env = p101_env_create(err, true, NULL);

    if(p101_error_has_error(err))
    {
        ctx.exit_code = EXIT_FAILURE;
        goto free_fsm_error;
    }

    p101_memset(env, &args, 0, sizeof(args));
    p101_memset(env, &ctx, 0, sizeof(ctx));
    ctx.arguments       = &args;
    ctx.arguments->argc = argc;
    ctx.arguments->argv = argv;
    ctx.exit_code       = EXIT_SUCCESS;

    fsm = p101_fsm_info_create(env, err, "elf-inspect-d-fsm", fsm_env, fsm_err, NULL);

    p101_fsm_info_set_bad_change_state_notifier(fsm, p101_fsm_info_default_bad_change_state_notifier);
    // p101_fsm_info_set_will_change_state_notifier(fsm, p101_fsm_info_default_will_change_state_notifier);
    p101_fsm_info_set_did_change_state_notifier(fsm, p101_fsm_info_default_did_change_state_notifier);

    p101_fsm_run(fsm, &from_state, &to_state, &ctx, transitions, sizeof(transitions));
    p101_fsm_info_destroy(env, &fsm);

    free(fsm_env);

free_fsm_error:
    p101_error_reset(fsm_err);
    p101_free(env, fsm_err);

free_env:
    p101_free(env, env);

free_error:
    p101_error_reset(err);
    free(err);

done:
    return ctx.exit_code;
}

static p101_fsm_state_t parse_arguments(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    struct contextd *context;
    p101_fsm_state_t next_state;
    int              opt;

    P101_TRACE(env);
    context                          = (struct contextd *)ctx;
    context->arguments->program_name = context->arguments->argv[0];
    next_state                       = HANDLE_ARGS;
    opterr                           = 0;

    while((opt = p101_getopt(env, context->arguments->argc, context->arguments->argv, "h")) != -1 && p101_error_has_no_error(err))
    {
        switch(opt)
        {
            case 'h':
            {
                next_state = USAGE;
                break;
            }
            case '?':
            {
                char msg[ERR_MSG_LEN];

                if(isprint(optopt))
                {
                    snprintf(msg, sizeof msg, "Unknown option '-%c'.", optopt);
                }
                else
                {
                    snprintf(msg, sizeof msg, "Unknown option character 0x%02X.", (unsigned)(unsigned char)optopt);
                }

                P101_ERROR_RAISE_USER(err, msg, ERRD_USAGE);
                break;
            }
            default:
            {
                P101_ERROR_RAISE_USER(err, "Unknown getopt failure", ERRD_USAGE);
                break;
            }
        }
    }

    if(p101_error_has_no_error(err))
    {
        if(optind >= context->arguments->argc)
        {
            P101_ERROR_RAISE_USER(err, "Socket path must be specified", ERRD_USAGE);
        }
        else if(optind < context->arguments->argc - 1)
        {
            P101_ERROR_RAISE_USER(err, "To many Arguments", ERRD_USAGE);
        }
        else
        {
            context->arguments->socket_path = context->arguments->argv[optind];
        }
    }

    if(p101_error_has_error(err))
    {
        next_state = USAGE;
    }

    return next_state;
}

static p101_fsm_state_t handle_arguments(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    struct contextd *context;
    p101_fsm_state_t next_state;
    int              socket_fd;

    P101_TRACE(env);
    context    = (struct contextd *)ctx;
    next_state = WAIT_FOR_REQUEST;

    unlink(context->arguments->socket_path);

    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(socket_fd == -1)
    {
        P101_ERROR_RAISE_USER(err, "Failed to create socket", ERRD_USAGE);
    }
    else
    {
        struct sockaddr_un addr;
        p101_memset(env, &addr, 0, sizeof(addr));
        context->socket_fd = socket_fd;

        if(init_sockaddr_un(&addr, context->arguments->socket_path) == -1)
        {
            P101_ERROR_RAISE_USER(err, "Socket path too long", ERRD_SOCKET);
        }
        else if(bind(socket_fd, (struct sockaddr *)&addr, sizeof addr) == -1)
        {
            P101_ERROR_RAISE_USER(err, "Failed to bind socket", ERRD_SOCKET);
        }
        else if(listen(context->socket_fd, SOCK_QUEUE) == -1)
        {
            P101_ERROR_RAISE_USER(err, "Failed to listen to socket", ERRD_SOCKET);
        }
    }

    if(p101_error_is_error(err, P101_ERROR_USER, ERRD_USAGE))
    {
        next_state = USAGE;
    }
    else if(p101_error_has_error(err))
    {
        next_state = CLEANUP_PROGRAM;
    }

    return next_state;
}

static p101_fsm_state_t wait_for_request(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    struct contextd *context;
    p101_fsm_state_t next_state;
    int              request_fd;

    P101_TRACE(env);
    context    = (struct contextd *)ctx;
    next_state = PARSE_REQUEST;

    request_fd = accept(context->socket_fd, NULL, NULL);

    if(exit_flag == 1)
    {
        next_state = CLEANUP_PROGRAM;
    }
    else if(request_fd == -1)
    {
        P101_ERROR_RAISE_USER(err, "Failed to accept request", ERRD_SOCKET);
        next_state = CLEANUP_PROGRAM;
    }
    else
    {
        context->request_fd = request_fd;
    }

    return next_state;
}

static p101_fsm_state_t parse_request(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    struct contextd *context;
    p101_fsm_state_t next_state;
    char             name[MAX_FILE_NAME_LEN];
    char             data[MAX_FILE_LEN + 1];
    ssize_t          readName;
    ssize_t          readData;

    P101_TRACE(env);
    context = (struct contextd *)ctx;
    p101_memset(env, &name, 0, sizeof(name));
    p101_memset(env, &data, 0, sizeof(data));
    next_state                     = VERIFY_ELF_HEADER;
    context->elf_details.valid_elf = true;

    readName = safe_read_line(context->request_fd, name, sizeof(name), true);
    readData = safe_read(context->request_fd, data, sizeof(data), true);

    if(readName == -1 || name[readName] != '\n')
    {
        P101_ERROR_RAISE_USER(err, "Bad request: Unparsable file name", ERRD_REQUEST);
    }
    else if(readData == -1)
    {
        P101_ERROR_RAISE_USER(err, "Bad request: Unparsable file data", ERRD_REQUEST);
    }
    else if(readData == sizeof(data))
    {
        P101_ERROR_RAISE_USER(err, "Bad request: File data too large", ERRD_REQUEST);
    }
    else
    {
        name[readName]                 = '\0';
        context->elf_details.file_name = p101_strdup(env, err, name);
        context->elf_details.size      = readData;

        if(context->elf_details.size < ELF32_HEADER_LEN)
        {
            P101_ERROR_RAISE_USER(err, "File data too short to be ELF32", ERRD_ELF);
        }
        else
        {
            switch(data[CLASS_LOCATION])
            {
                case ELFCLASS32:
                    p101_memcpy(env, &context->elf_details.header.elf_32, data, ELF32_HEADER_LEN);
                    context->elf_details.class = ELFCLASS32;
                    break;
                case ELFCLASS64:
                    if(context->elf_details.size < ELF64_HEADER_LEN)
                    {
                        P101_ERROR_RAISE_USER(err, "File data too short to be ELF64", ERRD_ELF);
                    }
                    else
                    {
                        p101_memcpy(env, &context->elf_details.header.elf_64, data, ELF64_HEADER_LEN);
                        context->elf_details.class = ELFCLASS64;
                    }
                    break;
                default:
                    P101_ERROR_RAISE_USER(err, "Unknown ELF class", ERRD_ELF);
                    break;
            }
        }
    }

    if(p101_error_is_error(err, P101_ERROR_USER, ERRD_REQUEST) || p101_error_is_error(err, P101_ERROR_USER, ERRD_ELF))
    {
        next_state = RESPOND;
    }
    if(p101_error_has_error(err))
    {
        next_state = CLEANUP_PROGRAM;
    }

    return next_state;
}

static p101_fsm_state_t verify_elf_header(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    struct contextd *context;
    elf_ident        ident;
    uint64_t         type;
    uint64_t         machine;
    uint64_t         entry;
    char             address[MAX_ELF_ADDRESS_CHARS];

    P101_TRACE(env);
    context = (struct contextd *)ctx;

    if(context->elf_details.class == ELFCLASS32)
    {
        ident   = context->elf_details.header.elf_32.e_ident;
        type    = context->elf_details.header.elf_32.e_type;
        machine = context->elf_details.header.elf_32.e_machine;
        entry   = context->elf_details.header.elf_32.e_entry;
    }
    else
    {
        ident   = context->elf_details.header.elf_64.e_ident;
        type    = context->elf_details.header.elf_64.e_type;
        machine = context->elf_details.header.elf_64.e_machine;
        entry   = context->elf_details.header.elf_64.e_entry;
    }

    if((determine_endianness() && verify_data(ident.ei_data, NULL) == ELFDATA2MSB) || (!determine_endianness() && verify_data(type, NULL) == ELFDATA2LSB))
    {
        byte_swap(&type, sizeof(type));
        byte_swap(&machine, sizeof(machine));
        byte_swap(&entry, sizeof(entry));
    }

    if(verify_magic(ident.ei_mag, NULL) == -1)
    {
        P101_ERROR_RAISE_USER(err, "Bad magic number", ERRD_ELF);
    }
    else
    {
        struct verification_pair pairs[] = {
            {verify_class,   ident.ei_class  },
            {verify_data,    ident.ei_data   },
            {verify_version, ident.ei_version},
            {verify_type,    type            },
            {verify_machine, machine         }
        };

        for(size_t i = 0; i < sizeof(pairs) / sizeof(pairs[0]); i++)
        {
            char msg[MAX_VALIDATION_MSG];
            if(pairs[i].verifier(pairs[i].input, msg) == -1)
            {
                P101_ERROR_RAISE_USER(err, msg, ERRD_ELF);
                break;
            }
        }
    }

    sprintf(address, "%#lx", entry);
    context->elf_details.entry_point = p101_strdup(env, err, address);

    return RESPOND;
}

static p101_fsm_state_t respond(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    struct contextd *context;

    P101_TRACE(env);
    context = (struct contextd *)ctx;

    if(p101_error_is_error(err, P101_ERROR_USER, ERRD_REQUEST))
    {
        const char *msg;
        msg = p101_error_get_message(err);
        safe_write(context->request_fd, msg, p101_strlen(env, msg));
    }
    else if(p101_error_is_error(err, P101_ERROR_USER, ERRD_ELF))
    {
        char msg[RESPONSE_MSG_LEN];
        snprintf(msg, sizeof(msg), "File: %s\nValid ELF: no\nError: %s\n", context->elf_details.file_name, p101_error_get_message(err));
        safe_write(context->request_fd, msg, p101_strlen(env, msg));
    }
    else
    {
        char msg[RESPONSE_MSG_LEN];
        snprintf(msg,
                 sizeof(msg),
                 "File: %s\nValid ELF: yes\nClass: %s\nEndianness: %s\nType: %s\nMachine: %s\nEntry point: %s",
                 context->elf_details.file_name,
                 context->elf_details.class_name,
                 context->elf_details.data_name,
                 context->elf_details.type_name,
                 context->elf_details.machine_name,
                 context->elf_details.entry_point);
        safe_write(context->request_fd, msg, p101_strlen(env, msg));
    }

    if(socket_close)
    {
        fputs("Client socket closed, did not send response", stderr);
        socket_close = 0;
    }

    shutdown(context->request_fd, SHUT_RDWR);
    p101_error_reset(err);

    return CLEANUP_RESPONSE;
}

void free_if_not_null(const struct p101_env *env, char **buf)
{
    if(*buf != NULL)
    {
        p101_free(env, *buf);
        *buf = NULL;
    }
}

static p101_fsm_state_t cleanup_response(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    struct contextd *context;
    p101_fsm_state_t next_state;

    P101_TRACE(env);
    context    = (struct contextd *)ctx;
    next_state = WAIT_FOR_REQUEST;

    free_if_not_null(env, &context->response_message);
    free_if_not_null(env, &context->elf_details.file_name);
    free_if_not_null(env, &context->elf_details.class_name);
    free_if_not_null(env, &context->elf_details.data_name);
    free_if_not_null(env, &context->elf_details.type_name);
    free_if_not_null(env, &context->elf_details.machine_name);
    free_if_not_null(env, &context->elf_details.entry_point);
    free_if_not_null(env, &context->elf_details.error);
    p101_memset(env, &context->elf_details, 0, sizeof(context->elf_details));

    p101_close(env, err, context->request_fd);

    if(p101_error_has_error(err))
    {
        next_state = CLEANUP_PROGRAM;
    }

    return next_state;
}

static p101_fsm_state_t usage(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    struct contextd *context;

    P101_TRACE(env);
    context = (struct contextd *)ctx;

    if(p101_error_has_error(err))
    {
        const char *msg;
        msg = p101_error_get_message(err);

        if(msg != NULL)
        {
            fputs(msg, stderr);
            fputc('\n', stderr);
        }

        p101_error_reset(err);
        context->exit_code = EXIT_FAILURE;
    }

    fprintf(stderr, "Usage: %s [-h] <socket-path>\n", context->arguments->program_name);
    fputs("Options:\n", stderr);
    fputs(" -h Display this help message\n", stderr);

    return CLEANUP_PROGRAM;
}

static p101_fsm_state_t cleanup_program(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    struct contextd *context;

    P101_TRACE(env);
    context = (struct contextd *)ctx;

    if(p101_error_has_error(err))
    {
        const char *msg;
        msg = p101_error_get_message(err);

        if(msg != NULL)
        {
            fputs(msg, stderr);
            fputc('\n', stderr);
        }

        p101_error_reset(err);
        context->exit_code = EXIT_FAILURE;
    }

    free_if_not_null(env, &context->response_message);
    free_if_not_null(env, &context->elf_details.file_name);
    free_if_not_null(env, &context->elf_details.class_name);
    free_if_not_null(env, &context->elf_details.data_name);
    free_if_not_null(env, &context->elf_details.type_name);
    free_if_not_null(env, &context->elf_details.machine_name);
    free_if_not_null(env, &context->elf_details.entry_point);
    free_if_not_null(env, &context->elf_details.error);

    p101_memset(env, &context->elf_details, 0, sizeof(context->elf_details));

    if(context->request_fd != 0)
    {
        p101_close(env, err, context->request_fd);
        context->request_fd = 0;
    }
    if(context->socket_fd != 0)
    {
        p101_close(env, err, context->socket_fd);
        context->socket_fd = 0;
    }

    if(p101_error_has_error(err))
    {
        fputs(p101_error_get_message(err), stderr);
        p101_error_reset(err);
        context->exit_code = EXIT_FAILURE;
    }

    return P101_FSM_EXIT;
}
