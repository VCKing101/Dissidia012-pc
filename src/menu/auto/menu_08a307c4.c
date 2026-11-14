#include "menu/auto_region1.h"

extern const short D_08A35580[];

/* Screen35550: clears field_6D3, frees its texts, plays the release script
 * D_08A35580 and goes to state 5. */
void func_08A307C4(Screen35550 *self)
{
    self->work->field_6D3 = 0;
    func_08A419A4(D_08A457EC);
    func_08A2EB10(self);
    func_088D3C24(self->work->layout, self->work->handles, D_08A35580, 0x120);
    self->work->state = 5;
}
