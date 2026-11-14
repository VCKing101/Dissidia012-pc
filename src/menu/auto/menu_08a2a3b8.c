#include "menu/auto_region1.h"

/* ScreenE75C8: hides entry index of its four handle rows. */
void func_08A2A3B8(ScreenE75C8 *self, int index)
{
    func_088D9CFC(self->work->field_470[index]);
    func_088D9CFC(self->work->field_48C[index]);
    func_088D9CFC(self->work->field_4A8[index]);
    func_088D9CFC(self->work->field_4C4[index]);
}
