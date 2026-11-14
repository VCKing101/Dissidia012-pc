#include "menu/auto_region1.h"

extern const unsigned short D_08A0963C[];

/* Screen0B6A0: clears the second handle set, plays D_08A0963C on its texts
 * and resets fields 38A0..38A8 and 2A608. */
void func_089FB708(Screen0B6A0 *self)
{
    func_088D3B5C(self->work->handles2, 0x40);
    func_088D3F7C(self->work->messages, self->work->texts, D_08A0963C, 0);
    self->work->field_38A4 = 0;
    self->work->field_38A0 = 0;
    self->work->field_38A8 = -1;
    self->work->field_2A608 = 0;
}
