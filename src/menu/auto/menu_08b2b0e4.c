#include "menu/auto_region4.h"

/* Obj29F3C: sets up the block at field_81C and its two callbacks. */
void func_08B2B0E4(Obj29F3C *self)
{
    self->work->field_850 = self->work->field_770;
    self->work->field_85C = func_08B2BAEC;
    self->work->field_860 = func_08B2BB0C;
    func_08935828(self->work->field_81C, 0, 0x34);
    func_08B58B64(D_08B59230, self->work->field_78C);
}
