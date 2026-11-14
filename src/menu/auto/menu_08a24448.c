#include "menu/auto_region1.h"

/* Screen28560: shows handle 0x24, then func_08A24A60 and func_08A244A4. */
void func_08A24448(Screen28560 *self)
{
    func_088D9CD8(self->work->handles[0x24]);
    func_08A24A60(self);
    func_08A244A4(self);
}
