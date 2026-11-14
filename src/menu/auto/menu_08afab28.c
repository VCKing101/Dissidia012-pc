#include "menu/auto_region4.h"

extern const unsigned short D_08B00F96[];
extern const unsigned short D_08B01114[];

void func_08AFAB28(ScreenB01CF8X *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08B00F96);
    func_088D428C(self->work->handles, self->work->texts, D_08B01114, 1);
    func_088D4928((MenuCursor *)&self->work->field_2E38, 0);
    func_08AFAB94(self);
}
