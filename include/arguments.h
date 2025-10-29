#ifndef ARGUMENTS_H
#define ARGUMENTS_H

struct arguments
{
    int argc;
    const char *program_name;
    const char *socket_path;
    const char *elf_path;
    char **argv;
};

#endif    // ARGUMENTS_H
