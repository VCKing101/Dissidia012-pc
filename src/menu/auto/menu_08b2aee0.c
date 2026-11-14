#include "menu/auto_region4.h"

extern const unsigned short D_08B2D0D4[];
extern const unsigned short D_08B2D104[];

void func_08B2AEE0(Obj29F3C *self)
{
    func_088D46E4(&self->work->cursor, 0, 0, 2, 0, -1);
    func_088D3CF0(self->work->layout, self->work->handles, D_08B2D0D4);
    ((MenuModelView *)self->work->handles[0x10])->field_0E = 7;
    func_088D428C(self->work->handles, self->work->texts, D_08B2D104, 1);
    func_088D9CFC(self->work->handles[0xE]);
    func_088D9CFC(self->work->handles[0xF]);
    func_088D9CD8(self->work->handles[0x14]);
    func_08B2AF98(self);
    func_08B2B048(self);
}
