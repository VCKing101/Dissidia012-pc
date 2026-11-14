#include "menu/auto_region4.h"

extern const unsigned short D_08B31826[];

void func_08B30334(Obj2F8F4 *self)
{
    Work2F8F4 *work;

    func_088D44EC(self->work->handles, self->work->texts, D_08B31826);
    work = self->work;
    func_088D3AE8(work->handles, work->field_938 * 2 + work->field_964 + 0x3F, 3, 0x50);
}
