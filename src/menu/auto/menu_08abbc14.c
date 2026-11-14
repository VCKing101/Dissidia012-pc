#include "menu/auto_region3.h"

/* ScreenC4560: copies 0x1A bytes from D_089D712C + 0x1FE08 to field_B2A. */
void func_08ABBC14(ScreenC4560 *self)
{
    func_08935524(self->work->field_B2A, ((Global9D712CView *)D_089D712C)->field_1FE08, 0x1A);
}
