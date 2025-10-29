#ifndef ELF_FILE_DETAILS_H
#define ELF_FILE_DETAILS_H

#include "elf32_header.h"
#include "elf64_header.h"
#include <stdbool.h>
#include <unistd.h>

struct elf_file_details
{
    ssize_t size;
    uint8_t class;

    union
    {
        elf32_header elf_32;
        elf64_header elf_64;
    } header;

    bool valid_elf;
    char *file_name;
    char *class_name;
    char *data_name;
    char *type_name;
    char *machine_name;
    char *entry_point;
    char *error;

};

#endif    // ELF_FILE_DETAILS_H
