#ifndef CONTEXT_H
#define CONTEXT_H

#include "arguments.h"

struct context
{
    struct arguments *arguments;

    int socket_fd;
    int elf_fd;

    int exit_code;
};

#endif    // CONTEXT_H
