#include "main/auto_region2.h"

/* Allocates a copy of the size bytes at src; data points 8 bytes into it. */
void func_0884C588(Obj84C4DC *self, const void *src, unsigned int size)
{
    char *buffer;

    buffer = func_088820D8(size);
    self->buffer = buffer;
    func_08935524(buffer, src, size);
    self->data = self->buffer + 8;
}
