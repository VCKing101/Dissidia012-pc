#include "menu/auto_region4.h"

extern unsigned char D_08B54B80[];

/* ScreenB54C38 constructor. */
ScreenB54C38 *func_08B54060(ScreenB54C38 *self)
{
    func_08864FF0(&self->task, 1, 0x11186, 0x50);
    self->field_134 = 0;
    self->field_138 = 0;
    self->field_13C = 0;
    self->field_140 = 0;
    self->field_144 = 0;
    self->field_148 = 0;
    self->field_14C = 0;
    self->field_14D = 0;
    self->field_14E = 0;
    self->link.vtable = D_08B54B80;
    self->link.owner = self;
    self->field_158 = 0;
    return self;
}
