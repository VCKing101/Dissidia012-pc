#include "menu/auto_region1.h"

extern const unsigned short D_08A0A0E2[];
extern const unsigned short D_08A0A0F2[];

/* Screen0B6A0: plays D_08A0A0E2, makes text 0x17 from message 0x7F, plays
 * the text script D_08A0A0F2. */
void func_089F4DB4(Screen0B6A0 *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08A0A0E2);
    self->work->texts[0x17] = func_089EF1E8(self, 0, 0x7F, 0, 0);
    func_088D428C(self->work->handles, self->work->texts, D_08A0A0F2, 1);
}
