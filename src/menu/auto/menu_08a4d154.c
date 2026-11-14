#include "menu/auto_region2.h"

/* Screen4FFF4: sets up the name input block (buffer, callbacks) and starts it. */
void func_08A4D154(Screen4FFF4 *self)
{
    self->work->field_6DF8 = self->work->field_6B32;
    self->work->field_6E04 = func_08A4D610;
    self->work->field_6E08 = func_08A4D630;
    func_08935828(self->work->field_6DC4, 0, 0x34);
    func_08B58B64(D_08B59230, &self->work->field_6D34);
}
