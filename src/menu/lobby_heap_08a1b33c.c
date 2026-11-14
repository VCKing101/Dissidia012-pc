#include "menu/agent29_types.h"

/* MenuRoot: allocates its buffer and builds the lobby heap over it. */
extern void *func_0888204C(int size);
extern void *func_0885C40C(void *buffer, int size, const char *name, int align);
extern const char D_08A239D8[];

void func_08A1B33C(MenuRoot *self)
{
    int size = 0x270800;
    self->field_134 = func_0888204C(size);
    self->heap = func_0885C40C(self->field_134, size, D_08A239D8, 0x40);
}
