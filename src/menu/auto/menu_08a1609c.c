#include "menu/auto_region1.h"

/* Screen181E0: hands its layout to the menu root (func_08A1E6B4, mode 0). */
void func_08A1609C(Screen181E0 *self)
{
    Work181E0 *work;

    work = self->work;
    func_08A1E6B4(D_08A23FF0, 0, &work->layout, work->handles, work->texts, &work->field_CDC);
}
