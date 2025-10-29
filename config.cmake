set(PROJECT_NAME "Comp3980Assignment2")
set(PROJECT_VERSION "1.0.0")
set(PROJECT_DESCRIPTION "Comp3980Assignment2")
set(PROJECT_LANGUAGE "C")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Common compiler flags
set(STANDARD_FLAGS
        -D_POSIX_C_SOURCE=200809L
        -D_XOPEN_SOURCE=700
        #-D_GNU_SOURCE
        #-D_DARWIN_C_SOURCE
        #-D__BSD_VISIBLE
        -Werror
)

# Define targets
set(EXECUTABLE_TARGETS
        elfinspectd
        elfinspect
)
set(LIBRARY_TARGETS "")

set(elfinspectd_SOURCES
        src/elfinspectd.c
        src/elf_validator.c
        src/util.c
)

set(elfinspectd_HEADERS
        include/argumentsd.h
        include/errorsd.h
        include/contextd.h
        include/elf_file_details.h
        include/elf_ident.h
        include/elf32_header.h
        include/elf64_header.h
        include/elf_validator.h
)

set(elfinspectd_LINK_LIBRARIES
        p101_error
        p101_env
        p101_c
        p101_posix
        p101_unix
        p101_fsm
        p101_convert
        m
)

set(elfinspect_SOURCES
        src/elfinspect.c
        src/util.c
)

set(elfinspect_HEADERS
        include/arguments.h
        include/context.h
        include/errors.h
)

set(elfinspect_LINK_LIBRARIES
        p101_error
        p101_env
        p101_c
        p101_posix
        p101_unix
        p101_fsm
        p101_convert
        m
)