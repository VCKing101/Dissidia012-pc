#include "menu/auto_region3.h"

/* ScreenDB30: reads field_AAC8 back from D_08B01CF8 and refreshes. */
void func_08ACEFA4(ScreenDB30 *self)
{
    func_08AD6D24(self);
    func_08AF4B6C(D_08B01CF8, self->work->field_AAC8, 0);
    func_08ACEEDC(self);
    func_08AD6D24(self);
    func_08AD1514(self, 1);
}
