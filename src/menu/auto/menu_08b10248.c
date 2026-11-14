#include "menu/auto_region4.h"

/* ScreenB1CB00: sets up the block at field_24C50 and its two callbacks. */
void func_08B10248(ScreenB1CB00 *self)
{
    self->work->field_24C84 = self->work->field_249BC;
    self->work->field_24C90 = func_08B103E4;
    self->work->field_24C94 = func_08B10404;
    func_08935828(self->work->field_24C50, 0, 0x34);
    func_08B58B64(D_08B59230, self->work->field_24BC0);
}
