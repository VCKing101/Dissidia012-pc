#include "menu/auto_region4.h"

void func_08B092B8(ScreenB1CB00 *self)
{
    func_08B08D04(self);
    self->work->flags = self->work->flags & 0xFFFFFBFB;
    func_08B0973C(self);
}
