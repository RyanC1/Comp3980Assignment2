#ifndef ELF64_HEADER_H
#define ELF64_HEADER_H

#include "elf_ident.h"
#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
    #endif

    #define ELF64_HEADER_LEN 64 // NOLINT(cppcoreguidelines-macro-to-enum,modernize-macro-to-enum)

    typedef struct {
        elf_ident e_ident;
        uint16_t e_type;
        uint16_t e_machine;
        uint32_t e_version;
        uint64_t e_entry;
        uint64_t e_phoff;
        uint64_t e_shoff;
        uint32_t e_flags;
        uint16_t e_ehsize;
        uint16_t e_phentsize;
        uint16_t e_phnum;
        uint16_t e_shentsize;
        uint16_t e_shnum;
        uint16_t e_shstrndx;
    } elf64_header;
    #ifdef __cplusplus
    #define C_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
    #define C_ALIGNOF(T) alignof(T)
    #else
    #define C_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
    #define C_ALIGNOF(T) _Alignof(T)
    #endif

    C_STATIC_ASSERT(sizeof(elf64_header) == ELF64_HEADER_LEN, "elf64_header size mismatch, expected 52");
    C_STATIC_ASSERT(C_ALIGNOF(elf64_header) == 8, "elf64_header must be 4-byte aligned");
    C_STATIC_ASSERT(offsetof(elf64_header, e_ident) == 0, "e_ident magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_type) == 16, "e_type magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_machine) == 18, "e_machine magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_version) == 20, "e_version magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_entry) == 24, "e_entry magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_phoff) == 32, "e_phoff magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_shoff) == 40, "e_shoff magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_flags) == 48, "e_flags magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_ehsize) == 52, "e_ehsize magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_phentsize) == 54, "e_phentsize magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_phnum) == 56, "e_phnum magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_shentsize) == 58, "e_shentsize magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_shnum) == 60, "e_shnum magic bad offset");
    C_STATIC_ASSERT(offsetof(elf64_header, e_shstrndx) == 62, "e_shstrndx magic bad offset");

    #undef C_STATIC_ASSERT
    #undef C_ALIGNOF

    #ifdef __cplusplus
    extern "C"
    }
#endif

#endif // ELF64_HEADER_H
