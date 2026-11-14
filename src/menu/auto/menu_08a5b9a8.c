#include "menu/auto_region2.h"

/* Obj5B958: keeps heap and data, sets up the block +0x410 on its layout. */
void func_08A5B9A8(Obj5B958 *self, void *heap, int arg, Obj56C *data)
{
    self->heap = heap;
    self->field_56C = data;
    func_08A5BC98(self->field_410, &self->layout);
}
