#ifndef ELF32_HEADER_H
#define ELF32_HEADER_H

#include "elf_ident.h"
#include <stddef.h>
#include <stdint.h>

#define ELF32_HEADER_LEN 52    // NOLINT(cppcoreguidelines-macro-to-enum,modernize-macro-to-enum)

typedef struct
{
    elf_ident e_ident;
    uint16_t  e_type;
    uint16_t  e_machine;
    uint32_t  e_version;
    uint32_t  e_entry;
    uint32_t  e_phoff;
    uint32_t  e_shoff;
    uint32_t  e_flags;
    uint16_t  e_ehsize;
    uint16_t  e_phentsize;
    uint16_t  e_phnum;
    uint16_t  e_shentsize;
    uint16_t  e_shnum;
    uint16_t  e_shstrndx;
} elf32_header;
#ifdef __cplusplus
    #define C_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
    #define C_ALIGNOF(T) alignof(T)
#else
    #define C_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
    #define C_ALIGNOF(T) _Alignof(T)
#endif

C_STATIC_ASSERT(sizeof(elf32_header) == ELF32_HEADER_LEN, "elf32_header size mismatch, expected 52");
C_STATIC_ASSERT(C_ALIGNOF(elf32_header) == 4, "elf32_header must be 4-byte aligned");
C_STATIC_ASSERT(offsetof(elf32_header, e_ident) == 0, "e_ident magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_type) == 16, "e_type magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_machine) == 18, "e_machine magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_version) == 20, "e_version magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_entry) == 24, "e_entry magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_phoff) == 28, "e_phoff magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_shoff) == 32, "e_shoff magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_flags) == 36, "e_flags magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_ehsize) == 40, "e_ehsize magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_phentsize) == 42, "e_phentsize magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_phnum) == 44, "e_phnum magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_shentsize) == 46, "e_shentsize magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_shnum) == 48, "e_shnum magic bad offset");
C_STATIC_ASSERT(offsetof(elf32_header, e_shstrndx) == 50, "e_shstrndx magic bad offset");

#undef C_STATIC_ASSERT
#undef C_ALIGNOF

#endif    // ELF32_HEADER_H
