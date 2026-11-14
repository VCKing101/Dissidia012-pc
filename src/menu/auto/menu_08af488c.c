#include "menu/auto_region3.h"

/* ScreenB01CF8: keeps func_089203BC's value for D_089D712C + 0x3F548. */
void func_08AF488C(ScreenB01CF8 *self)
{
    int value;

    value = func_089203BC(((Global9D712CView *)D_089D712C)->field_3F548);
    self->work->field_2E94 = value;
}
