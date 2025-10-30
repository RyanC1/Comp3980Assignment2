#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <sys/un.h>
#include <unistd.h>

/**
 * Safely reads count bytes from the given file descriptor or until eof.
 * Returns the number of characters read or -1 if an error occurs.
 * Setting exact to true will prevent non-blocking io from returning
 * a value, instead returning -1.
 *
 * @param fd the file descriptor to read from
 * @param buf where to read too
 * @param count bytes to read
 * @param exact if non-blocking fds should return the number of bytes read or -1
 * @return number of bytes read or -1
 */
ssize_t safe_read(int fd, void *buf, size_t count, bool exact);

/**
 * Safely reads count bytes from the given file descriptor or until eof or newline.
 * Returns the number of characters read or -1 if an error occurs.
 * Setting exact to true will prevent non-blocking io from returning
 * a value, instead returning -1.
 *
 * @param fd the file descriptor to read from
 * @param buf where to read too
 * @param count bytes to read
 * @param exact if non-blocking fds should return the number of bytes read or -1
 * @return number of bytes read or -1
 */
ssize_t safe_read_line(int fd, void *buf, size_t count, bool exact);

/**
 * Writes n bytes from buf to the given fd.
 * Will return n or -1 if not all the bytes were written.
 * @param fd the file to write to
 * @param buf the buffer to pull from
 * @param n the number of bytes to write
 * @return n or -1 if partial write
 */
ssize_t safe_write(int fd, const void *buf, size_t n);

/**
 * Writes n bytes from buf to the given fd plus a terminating newline.
 * Will return (n + 1) or -1 if not all the bytes were written.
 * @param fd the file to write to
 * @param buf the buffer to pull from
 * @param n the number of bytes to write
 * @return (n + 1) or -1 if partial write
 */
ssize_t safe_write_line(int fd, const void *buf, size_t n);

/**
 * Copies the contents of one fd into another
 * Returns the number of bytes copied or -1 if it's a partial copy
 *
 * @param source the file to pull bytes from
 * @param destination the file to write to
 * @return the number of bytes copied or -1 if partial copy
 */
ssize_t copy(int source, int destination);

/**
 * Determines host endianess
 * @return 1 if small endian, 0 if big endian
 */
int determine_endianness(void);

/**
 * Reverses the bytes of the given buffer.
 * @param buf the buffer to reverse the bytes in
 * @param size the size of the buffer
 */
void byte_swap(void *buf, int size);

/**
 * Binds the given link to the given sockaddr_un, returning -1
 * if the link is too long.
 *
 * @param addr the sockaddr_un to bind the link
 * @param path the link to bind
 * @return 0 if successful, -1 if not
 */
int init_sockaddr_un(struct sockaddr_un *addr, const char *path);

#endif    // UTIL_H
