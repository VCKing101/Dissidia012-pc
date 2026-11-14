#include "menu/auto_region1.h"

/* ScreenE75C4: allocates its buffer and builds a heap over it. */
extern void *func_0888204C(int size);
extern void *func_0885C40C(void *buffer, int size, const char *name, int align);
extern const char D_089E7260[];

void func_089E1034(ScreenE75C4 *self)
{
    int size = 0x270800;
    self->field_134 = func_0888204C(size);
    self->heap = func_0885C40C(self->field_134, size, D_089E7260, 0x40);
}
