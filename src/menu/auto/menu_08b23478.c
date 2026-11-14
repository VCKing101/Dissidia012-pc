#include "menu/auto_region4.h"

extern const short D_08B23E22[];
extern const unsigned short D_08B23F12[];

void func_08B23478(ScreenB243C0 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08B23E22);
    func_08AF3930(D_08B01CF8, &self->work->layout, self->work->handles);
    func_08B23378(self);
    func_08B23300(self);
    func_088D44EC(self->work->handles, self->work->texts, D_08B23F12);
}
