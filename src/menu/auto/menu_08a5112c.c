#include "menu/auto_region2.h"

/* Screen51CBC: picks row index and loads it into the block +0x7BC0. */
void func_08A5112C(Screen51CBC *self, int index)
{
    self->work->field_79A4 = 1;
    self->work->row = &self->work->rows[index];
    func_088CBA10(self->work->field_7BC0);
    func_088B9E84(self->work->field_7BC0, self->work->row);
}
