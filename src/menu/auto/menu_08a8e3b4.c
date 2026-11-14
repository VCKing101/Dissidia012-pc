#include "menu/auto_region3.h"

/* View8E3B4: resets its window state and hides text 0x16. */
void func_08A8E3B4(View8E3B4 *self)
{
    int value;

    self->work->field_AD8 = 0;
    value = func_089C5FB8();
    self->work->field_ADC = value;
    self->work->field_AE0 = self->work->field_2FC;
    func_088D9F5C(self->work->handles[0x83], 0, 1);
    func_0887B944(self->work->texts[0x16]);
    self->work->flags = self->work->flags | 8;
    self->work->flags = self->work->flags & 0xFFFFFFEF;
}
