#ifndef CONTEXTD_H
#define CONTEXTD_H

#include "argumentsd.h"
#include "elf_file_details.h"

struct contextd
{
    struct argumentsd *arguments;

    int socket_fd;
    int request_fd;
    struct elf_file_details elf_details;
    char* response_message;

    int exit_code;
};

#endif    // CONTEXTD_H
