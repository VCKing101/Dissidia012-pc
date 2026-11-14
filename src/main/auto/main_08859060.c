#include "main/auto_region2.h"

/* Constructor of Part859060: no handles yet. */
Part859060 *func_08859060(Part859060 *self)
{
    int **handle;
    int i;

    i = 0;
    handle = self->handles;
    do {
        *handle = 0;
        i = i + 1;
        handle = handle + 1;
    } while (i < 5);
    return self;
}
