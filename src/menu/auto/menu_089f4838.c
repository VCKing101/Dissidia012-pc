#include "menu/auto_region1.h"

extern const unsigned short D_08A09CFA[];
extern const unsigned short D_08A09D2A[];
extern const unsigned short D_08A0A102[];

/* Screen0B6A0: plays D_08A09CFA, makes texts 0x16/0x17 from messages
 * 0x7D/0x7E, plays D_08A09D2A and D_08A0A102. */
void func_089F4838(Screen0B6A0 *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08A09CFA);
    self->work->texts[0x16] = func_089EF1E8(self, 0, 0x7D, 0, 0);
    self->work->texts[0x17] = func_089EF1E8(self, 0, 0x7E, 0, 0);
    func_088D428C(self->work->handles, self->work->texts, D_08A09D2A, 1);
    func_088D3CF0(&self->work->layout, self->work->handles, D_08A0A102);
    func_089F3FC8(self, self->work->field_3158.field_14, 0xA5, 0, 0);
    func_089F3F68(self, 0xAA);
}
