#include "menu/auto_region3.h"

/* ScreenDB30: plays motion 0x1F on handle 0x23 and redraws. */
void func_08ACBFCC(ScreenDB30 *self)
{
    func_088D9F5C(self->work->handles[0x23], 0x1F, 1);
    func_08ACC020(self);
    func_08AD40C0(self);
    func_08ACF548(self);
}
