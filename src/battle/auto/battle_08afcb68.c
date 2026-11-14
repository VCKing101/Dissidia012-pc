#include "battle/auto_region5.h"

/* PartFCB68 constructor: clears it and sets field_B4 to 99999999. */
PartFCB68 *func_08AFCB68(PartFCB68 *self)
{
    self->field_00 = 0;
    self->field_04 = 0;
    self->field_C8 = 0;
    self->field_CC = 0;
    self->field_D4 = 0;
    self->field_D0 = 0;
    func_0885EAFC(self->field_E4);
    func_08935828(self->field_08, 0, 0xC0);
    self->field_B4 = 99999999;
    self->field_CC = 0;
    return self;
}
