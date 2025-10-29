#include "../include/util.h"
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>

ssize_t safe_read(const int fd, void *buf, const size_t count, bool exact)
{
    unsigned char *p;
    size_t         total;
    ssize_t        n;

    p     = buf;
    total = 0;

    do
    {
        n = read(fd, p + total, count - total);
        if(n > 0)
        {
            total += (size_t)n;
        }
        else if(n == -1 && errno != EINTR)
        {
            if(errno == EAGAIN && !exact && total > 0)
            {
                return (ssize_t)total;
            }
            return -1;
        }

    } while(total < count && n != 0);

    return (ssize_t)total;
}

ssize_t safe_read_line(const int fd, void *buf, const size_t count, bool exact)
{
    unsigned char *p;
    size_t         total;
    ssize_t        n;

    p     = buf;
    total = 0;

    do
    {
        n = read(fd, p + total, 1);
        if(n == 1 && p[total++] == '\n')
        {
            break;
        }
        if(n == -1 && errno != EINTR)
        {
            if(errno == EAGAIN && exact && total > 0)
            {
                return (ssize_t)total;
            }
            return -1;
        }

    } while(total < count && n != 0);

    return (ssize_t)total;
}

/*
 * Attempts to write exactly n bytes from buf to fd.
 * Returns:
 * n : success, all bytes written
 * -1 : error (errno set), caller can inspect errno
 */
ssize_t safe_write(int fd, const void *buf, size_t n)
{
    const unsigned char *p;
    size_t               left;

    p    = (const unsigned char *)buf;
    left = n;
    while(left > 0)
    {
        ssize_t w;
        w = write(fd, p, left);
        if(w > 0)
        {
            p += (size_t)w;
            left -= (size_t)w;
            continue;
        }
        if(w < 0 && errno == EINTR)
        {
            continue;
        }

        return -1;
    }
    return (ssize_t)n;
}

ssize_t safe_write_line(int fd, const void *buf, size_t n)
{
    ssize_t written;

    written = safe_write(fd, buf, n);

    if(written == -1 || safe_write(fd, "\n", 1))
    {
        return -1;
    }
    return written + 1;
}

ssize_t copy(int source, int destination)
{
    ssize_t read;
    ssize_t total;
    char    c[1];

    total = 0;

    do
    {
        read = safe_read(source, c, 1, true);
        if(read == -1)
        {
            return -1;
        }
        if(safe_write(destination, &c, (size_t)read) == -1)
        {
            return -1;
        }
        total++;
    } while(read != 0);

    return total;
}

int determine_endianness(void)
{
    const uint16_t v = 0x0102;
    const uint8_t *p = (const uint8_t *)&v;

    return p[0] == 0x02;
}

void byte_swap(void *buf, const int size)
{
    uint8_t  tmp[1];
    uint8_t *p;

    p = (uint8_t *)buf;

    for(int i = 0; i < size / 2; i++)
    {
        memcpy(tmp, p + i, 1);
        memcpy(p + size - 1 - i, p + i, 1);
        memcpy(p + i, tmp, 1);
    }
}

int init_sockaddr_un(struct sockaddr_un *addr, const char *path)
{
    addr->sun_family = AF_UNIX;
    if(strlen(path) >= sizeof(addr->sun_path))
    {
        return -1;
    }
    strncpy(addr->sun_path, path, sizeof(addr->sun_path));
    return 0;
}
