#ifndef VERIFICATION_SET_H
#define VERIFICATION_SET_H

#include <stdint.h>

struct verification_set
{
    int (*verifier)(uint64_t, char *);
    const uint64_t input;
    char         **buf;
};

#endif    // VERIFICATION_SET_H
