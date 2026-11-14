#include "menu/auto_region1.h"

/* Screen0B6A0: sets bit 1 of field_38A4, shows handles2 0xD/0xE when bit
 * 0x10 is set, and opens the prompt with 4. */
void func_089FE280(Screen0B6A0 *self)
{
    self->work->field_38A4 = self->work->field_38A4 | 1;
    if ((self->work->field_38A4 & 0x10) != 0) {
        func_088D9CD8(self->work->handles2[0xD]);
        func_088D9CD8(self->work->handles2[0xE]);
    }
    func_089FAE4C(self, 4);
}
