#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <sys/un.h>
#include <unistd.h>

/*
* safe_read: read up to 'count' bytes, handling EINTR and
* returning the total bytes read so far if the descriptor
* is nonblocking and would block. Returns:
* >= 0 : total bytes read (0 means EOF and nothing was
* read)
* -1 : error (errno is set). If some bytes were read
* before EAGAIN, those bytes are returned
instead
* of -1.
*/
ssize_t safe_read(int fd, void *buf, size_t count, bool exact);

ssize_t safe_read_line(int fd, void *buf, size_t count, bool exact);

ssize_t safe_write(int fd, const void *buf, size_t n);

ssize_t safe_write_line(int fd, const void *buf, size_t n);

ssize_t copy(int source, int destination);

int determine_endianness(void);

void byte_swap(void* buf, int size);

int init_sockaddr_un (struct sockaddr_un *addr, const char *path);


#endif    // UTIL_H
