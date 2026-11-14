#include "menu/auto_region3.h"

/* ScreenC4560: marks D_089D712C (unless field_B47), runs func_088AC268; state 0x1C. */
void func_08ABD234(ScreenC4560 *self)
{
    if (self->work->field_B47 == 0) {
        ((Global9D712CView *)D_089D712C)->field_EFD = 1;
    }
    func_088AC268(D_089D712C);
    self->work->field_B58 = 0;
    self->work->state = 0x1C;
}
