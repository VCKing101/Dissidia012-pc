#include "battle/agent27_types.h"

extern char *func_08B1948C(MemStream *stream, int size);
extern void func_08935524(void *dst, const void *src, int size);

int func_08B19508(MemStream *stream, void *dst, int size)
{
    char *src;

    if (stream->open == 0) {
        return -1;
    }
    if (dst == 0 || size <= 0) {
        return -2;
    }
    src = func_08B1948C(stream, size);
    if (src == 0) {
        return -3;
    }
    func_08935524(dst, src, size);
    return 0;
}
