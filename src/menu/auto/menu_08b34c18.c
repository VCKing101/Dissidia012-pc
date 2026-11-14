#include "menu/auto_region4.h"

/* ScreenB41CC0 constructor. */
ScreenB41CC0 *func_08B34C18(ScreenB41CC0 *self)
{
    func_08864FF0((MenuTask *)&self->task, 1, 0x11172, 0x54);
    self->field_58 = 0;
    self->field_90 = 0;
    self->field_94 = 0;
    func_08B35A64(self);
    func_08B35ABC(self);
    func_08B35A44(self);
    self->field_B8 = 0;
    self->field_BC = 0;
    self->field_C0 = 0;
    return self;
}
