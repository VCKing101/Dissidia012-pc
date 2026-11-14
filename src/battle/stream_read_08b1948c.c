#include "battle/agent27_types.h"

extern int func_08B1958C(MemStream *stream, int offset, int whence);

char *func_08B1948C(MemStream *stream, int size)
{
    char *data;

    if (stream->open == 0) {
        return 0;
    }
    if (size <= 0) {
        return 0;
    }
    data = stream->base + stream->pos;
    if (func_08B1958C(stream, size, 1) != 0) {
        return 0;
    }
    return data;
}
