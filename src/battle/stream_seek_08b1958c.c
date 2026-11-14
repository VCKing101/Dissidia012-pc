#include "battle/agent27_types.h"

int func_08B1958C(MemStream *stream, int offset, int whence)
{
    if (stream->open == 0) {
        return -1;
    }
    switch (whence) {
    case 0:
        break;
    case 1:
        offset += stream->pos;
        break;
    case 2:
        offset += stream->size;
        break;
    default:
        return -2;
    }
    {
        int pos = offset;
        if (pos < 0 || offset > stream->size) {
            return -3;
        }
        stream->pos = pos;
    }
    return 0;
}
