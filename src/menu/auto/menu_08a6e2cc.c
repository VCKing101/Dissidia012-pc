#include "menu/auto_region2.h"

/* Request6E168: sets kind and copies data when it fits in 0x40 bytes. */
void func_08A6E2CC(Request6E168 *self, int kind, const void *data, int size)
{
    self->kind = kind;
    if (data != 0) {
        if (size < 0x41) {
            func_08935524(self->data, data, size);
            self->size = size;
        } else {
            self->size = 0;
        }
    }
}
