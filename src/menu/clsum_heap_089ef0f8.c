#include "menu/agent29_types.h"

/* Screen0B6A0: allocates its buffer and builds the CLSUM heap over it. */
extern void *func_0888204C(int size);
extern void *func_0885C40C(void *buffer, int size, const char *name, int align);
extern const char D_08A0AC48[];

void func_089EF0F8(Screen0B6A0 *self)
{
    int size = 0x270800;
    self->field_134 = func_0888204C(size);
    self->heap = func_0885C40C(self->field_134, size, D_08A0AC48, 0x40);
}
