#include "menu/auto_region1.h"

extern const short D_08A09542[];

/* Screen0B6A0: opens the screen: plays D_08A09542, works out field_273C
 * (field_2734 percent of field_68's field_08), sets up cursor field_339C
 * over field_27C8 entries and goes to state 0xB. */
void func_089F9B04(Screen0B6A0 *self)
{
    Work0B6A0 *work;
    unsigned int *flags;

    func_0889F234(D_089D6C30, 0x45);
    func_088D3B88(&self->work->layout, self->work->handles, D_08A09542, 0, 0x180);
    work = self->work;
    work->field_273C = (unsigned int)work->field_2734 * (int)work->field_68->field_08;
    self->work->field_273C = self->work->field_273C / 100;
    self->work->field_2740 = D_089D712C->field_1DE00;
    func_08A05138(self);
    self->work->field_3238 = 0;
    func_088D46E4(&self->work->field_339C, 0, 0, self->work->field_27C8 - 1, 0, 5);
    flags = &self->work->field_339C.flags;
    *flags = *flags | 4;
    func_089F8B1C(self);
    self->work->state = 0xB;
    func_08A04AF8(self);
}
