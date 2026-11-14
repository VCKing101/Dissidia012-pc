#include "menu/auto_region3.h"

/* ScreenA690: sets flag 2 and redraws everything. */
void func_08AE6A20(ScreenA690 *self)
{
    self->work->flags = self->work->flags | 2;
    func_08AE5F20(self);
    func_08AE5F70(self);
    func_08AE6028(self);
    func_08AE4CC0(self);
    func_08AE61A8(self);
    func_08AE4D70(self);
}
