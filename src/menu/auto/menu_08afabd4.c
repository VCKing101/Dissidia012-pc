#include "menu/auto_region4.h"

void func_08AFABD4(ScreenB01CF8X *self)
{
    func_0887B944(self->work->texts[0x4F]);
    func_0887B924(self->work->texts[0x5A]);
    func_088D4928(&self->work->field_2E64, 0);
    self->work->field_2E90 = self->work->field_2E38.index;
    func_088D4788(&self->work->field_2E64, &self->work->field_2E90, 1);
    func_08AFAD60(self);
}
