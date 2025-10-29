#ifndef ELF_IDENT_H
#define ELF_IDENT_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif

enum {
    ELF_IDENT_MAGIC_LEN       = 4,
    ELF_IDENT_PADDING_LEN     = 7,
};

#define ELF_IDENT_LEN 16 // NOLINT(cppcoreguidelines-macro-to-enum,modernize-macro-to-enum)

typedef struct
{
    uint8_t ei_mag[ELF_IDENT_MAGIC_LEN]; /* 0x7F 'E' 'L' 'F' */
    uint8_t ei_class;
    uint8_t ei_data;
    uint8_t ei_version;
    uint8_t ei_osabi;
    uint8_t ei_abiversion;
    uint8_t ei_pad[ELF_IDENT_PADDING_LEN];
} elf_ident;

#ifdef __cplusplus
#define C_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#define C_ALIGNOF(T) alignof(T)
#else
#define C_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#define C_ALIGNOF(T) _Alignof(T)
#endif

C_STATIC_ASSERT(sizeof(elf_ident) == ELF_IDENT_LEN, "elf_ident size mismatch, expected 16");
C_STATIC_ASSERT(C_ALIGNOF(elf_ident) == 1, "elf_identi must be 1-byte aligned");
C_STATIC_ASSERT(offsetof(elf_ident, ei_mag[0]) == 0, "elf_ident magic bad offset");
C_STATIC_ASSERT(offsetof(elf_ident, ei_class) == 4, "elf_ident elf_class bad offset");
C_STATIC_ASSERT(offsetof(elf_ident, ei_data) == 5, "elf_ident data bad offset");
C_STATIC_ASSERT(offsetof(elf_ident, ei_version) == 6, "elf_ident version bad offset");
C_STATIC_ASSERT(offsetof(elf_ident, ei_osabi) == 7, "elf_ident osabi bad offset");
C_STATIC_ASSERT(offsetof(elf_ident, ei_abiversion) == 8, "elf_ident abiversion bad offset");
C_STATIC_ASSERT(offsetof(elf_ident, ei_pad[0]) == 9, "elf_ident pad bad offset");

#undef C_STATIC_ASSERT
#undef C_ALIGNOF

#ifdef __cplusplus
    extern "C"
    }
#endif

#endif // ELF_IDENT_H
