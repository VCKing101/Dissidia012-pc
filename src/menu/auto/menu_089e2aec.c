#include "menu/auto_region1.h"

/* ScreenE75C4: keeps the 16-bit position (x, y) and moves handle 0x31
 * there. */
void func_089E2AEC(ScreenE75C4 *self, unsigned int x, unsigned int y)
{
    self->work->field_A8C = x & 0xFFFF;
    self->work->field_A90 = y & 0xFFFF;
    func_088D3AE8(self->work->handles, x & 0xFFFF, y & 0xFFFF, 0x31);
    func_088D9A94(self->work->handles[0x31]);
}
