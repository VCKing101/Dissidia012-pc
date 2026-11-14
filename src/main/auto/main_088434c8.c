#include "main/auto_region2.h"

/* Builds one 0x50-byte object 8 bytes into the block at field_10 and points field_14 at it (0 without a block). */
void func_088434C8(Obj8434C8 *self)
{
    void *object;
    char *block;

    block = self->field_10;
    object = 0;
    if (block != 0) {
        func_08993560(block + 8, 1, 0x50, &D_08842F70);
        object = block;
    }
    self->field_14 = object;
}
