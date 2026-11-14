#include "menu/auto_region3.h"

/* ScreenDB30: copies three values from D_08B01CF8's data. */
void func_08ACADD4(ScreenDB30 *self)
{
    self->work->field_1418 = ((ScreenB01CF8 *)D_08B01CF8)->work->field_C2C;
    self->work->field_141C = ((ScreenB01CF8 *)D_08B01CF8)->work->field_C30;
    self->work->field_1420 = ((ScreenB01CF8 *)D_08B01CF8)->work->field_C18;
}
