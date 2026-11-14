#include "menu/auto_region1.h"

/* Screen181E0: hands its layout to the menu root (func_08A21C88) and plays
 * motion 0x3E on handle 1. */
void func_08A1602C(Screen181E0 *self)
{
    Work181E0 *work;

    work = self->work;
    func_08A21C88(D_08A23FF0, work->field_C7C, 0, -1, &work->layout, work->handles, work->texts,
                  &work->field_CDC, 1, 1, 1, 0);
    func_088D9F5C(self->work->handles[1], 0x3E, 1);
}
