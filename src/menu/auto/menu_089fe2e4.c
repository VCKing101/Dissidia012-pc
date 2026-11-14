#include "menu/auto_region1.h"

/* Screen0B6A0: clears bit 1 of field_38A4, hides handles2 0xD/0xE and opens
 * the prompt with 3. */
void func_089FE2E4(Screen0B6A0 *self)
{
    self->work->field_38A4 = self->work->field_38A4 & ~1;
    func_088D9CFC(self->work->handles2[0xD]);
    func_088D9CFC(self->work->handles2[0xE]);
    func_089FAE4C(self, 3);
}
