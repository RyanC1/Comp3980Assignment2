#include "arguments.h"
#include "context.h"
#include "errors.h"
#include "util.h"
#include <ctype.h>
#include <fcntl.h>
#include <p101_c/p101_stdlib.h>
#include <p101_c/p101_string.h>
#include <p101_fsm/fsm.h>
#include <p101_posix/p101_string.h>
#include <p101_posix/p101_unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>

enum states
{
    PARSE_ARGS = P101_FSM_USER_START,
    HANDLE_ARGS,
    USAGE,
    CONNECT,
    SEND_FILE,
    RECEIVE_DETAILS,
    CLEANUP,
};

static volatile sig_atomic_t socket_close = 0;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

static p101_fsm_state_t parse_arguments(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t handle_arguments(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t connect_to_server(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t send_file(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t receive_details(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t usage(const struct p101_env *env, struct p101_error *err, void *ctx);
static p101_fsm_state_t cleanup(const struct p101_env *env, struct p101_error *err, void *ctx);

#define ERR_MSG_LEN 256         // NOLINT(cppcoreguidelines-macro-to-enum, modernize-macro-to-enum)
#define MAX_RECEIVE_LEN 1028    // NOLINT(cppcoreguidelines-macro-to-enum, modernize-macro-to-enum)
#define EXPECTED_ARGS 2         // NOLINT(cppcoreguidelines-macro-to-enum, modernize-macro-to-enum)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void handle_signal(int sig)
{
    socket_close = 1;
}

#pragma GCC diagnostic pop

int main(int argc, char *argv[])
{
    static struct p101_fsm_transition transitions[] = {
        {P101_FSM_INIT,   PARSE_ARGS,      parse_arguments  },
        {PARSE_ARGS,      USAGE,           usage            },
        {PARSE_ARGS,      HANDLE_ARGS,     handle_arguments },
        {HANDLE_ARGS,     USAGE,           usage            },
        {HANDLE_ARGS,     CONNECT,         connect_to_server},
        {CONNECT,         SEND_FILE,       send_file        },
        {CONNECT,         CLEANUP,         cleanup          },
        {SEND_FILE,       RECEIVE_DETAILS, receive_details  },
        {RECEIVE_DETAILS, CLEANUP,         cleanup          },
        {USAGE,           CLEANUP,         cleanup          },
        {CLEANUP,         P101_FSM_EXIT,   NULL             }
    };

    struct p101_error    *err;
    struct p101_env      *env;
    struct p101_fsm_info *fsm;
    p101_fsm_state_t      from_state;
    p101_fsm_state_t      to_state;
    struct p101_error    *fsm_err;
    struct p101_env      *fsm_env;
    struct arguments      args;
    struct context        ctx;

    if(signal(SIGPIPE, handle_signal) == SIG_ERR)
    {
        perror("signal");
        exit(EXIT_FAILURE);
    }

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

    fsm = p101_fsm_info_create(env, err, "elf-inspect-fsm", fsm_env, fsm_err, NULL);

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
    struct context  *context;
    p101_fsm_state_t next_state;
    int              opt;

    P101_TRACE(env);
    context                          = (struct context *)ctx;
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

                P101_ERROR_RAISE_USER(err, msg, ERR_USAGE);
                break;
            }
            default:
            {
                P101_ERROR_RAISE_USER(err, "Unknown getopt failure", ERR_USAGE);
                break;
            }
        }
    }

    if(p101_error_has_no_error(err))
    {
        if(context->arguments->argc - optind != EXPECTED_ARGS)
        {
            P101_ERROR_RAISE_USER(err, "Incorrect number of arguments", ERR_USAGE);
        }
        else
        {
            context->arguments->socket_path = context->arguments->argv[optind];
            context->arguments->elf_path    = context->arguments->argv[optind + 1];
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
    struct context  *context;
    p101_fsm_state_t next_state;
    int              socket_fd;

    P101_TRACE(env);
    context    = (struct context *)ctx;
    next_state = CONNECT;

    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(socket_fd == -1)
    {
        P101_ERROR_RAISE_USER(err, "Failed to create socket", ERR_USAGE);
    }
    else
    {
        int elf_fd;
        elf_fd = open(context->arguments->elf_path, O_RDONLY | O_CLOEXEC);

        if(elf_fd == -1)
        {
            P101_ERROR_RAISE_USER(err, "Failed to open ELF file", ERR_USAGE);
        }
        else
        {
            struct stat file_stats;
            int         stat_status;

            stat_status = fstat(elf_fd, &file_stats);

            if(stat_status == -1)
            {
                P101_ERROR_RAISE_USER(err, "Failed to get fstat() of ELF file", ERR_USAGE);
            }
            else if(!S_ISREG(file_stats.st_mode))
            {
                P101_ERROR_RAISE_USER(err, "ELF file is not a regular file", ERR_USAGE);
            }

            context->elf_fd    = elf_fd;
            context->socket_fd = socket_fd;
        }
    }

    if(p101_error_has_no_error(err))
    {
        next_state = USAGE;
    }

    return next_state;
}

static p101_fsm_state_t connect_to_server(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    const struct context *context;
    p101_fsm_state_t      next_state;
    struct sockaddr_un    addr;

    P101_TRACE(env);
    context    = (struct context *)ctx;
    next_state = SEND_FILE;

    p101_memset(env, &addr, 0, sizeof(addr));

    if(init_sockaddr_un(&addr, context->arguments->socket_path) == -1)
    {
        P101_ERROR_RAISE_USER(err, "Socket path too long", ERR_SOCKET);
    }
    else if(connect(context->socket_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        P101_ERROR_RAISE_USER(err, "Failed to connect to server", ERR_SOCKET);
    }

    if(p101_error_has_error(err))
    {
        return CLEANUP;
    }

    return next_state;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static p101_fsm_state_t send_file(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    struct context *context;

    P101_TRACE(env);
    context = (struct context *)ctx;

    if(safe_write(context->socket_fd, context->arguments->elf_path, strlen(context->arguments->elf_path)) == -1)
    {
        puts("Notice: Failed to send full path name");
    }

    if(copy(context->elf_fd, context->socket_fd))
    {
        puts("Notice: Failed to send full elf data");
    }

    if(socket_close)
    {
        puts("Notice: Server closed socket mid write");
    }

    shutdown(context->socket_fd, SHUT_WR);

    return RECEIVE_DETAILS;
}

#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static p101_fsm_state_t receive_details(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    const struct context *context;
    char                  msg[MAX_RECEIVE_LEN + 1];
    ssize_t               read;

    P101_TRACE(env);
    context = (struct context *)ctx;
    p101_memset(env, msg, 0, sizeof(msg));

    puts("reading");
    read = safe_read(context->socket_fd, msg, sizeof(msg), true);
    if(read == sizeof(msg))
    {
        puts("Response too long!");
    }
    if(read == -1)
    {
        puts("Could not parse response");
    }

    puts("Server Response:");
    puts(msg);

    return CLEANUP;
}

#pragma GCC diagnostic pop

static p101_fsm_state_t usage(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    struct context *context;

    P101_TRACE(env);
    context = (struct context *)ctx;

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

    fprintf(stderr, "Usage: %s [-h] <socket-path> <elf-file-path>\n", context->arguments->program_name);
    fputs("Options:\n", stderr);
    fputs(" -h Display this help message\n", stderr);

    return CLEANUP;
}

static p101_fsm_state_t cleanup(const struct p101_env *env, struct p101_error *err, void *ctx)
{
    struct context *context;

    P101_TRACE(env);
    context = (struct context *)ctx;

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

    if(context->elf_fd != 0)
    {
        p101_close(env, err, context->elf_fd);
        context->elf_fd = 0;
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
