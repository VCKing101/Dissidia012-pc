#include "main/auto_region2.h"

/* Allocator callback: size bytes from the world heap. */
void *func_088312D0(void *self, unsigned int size)
{
    return func_08842100(size);
}
