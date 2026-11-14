#include "menu/auto_region1.h"

/* Screen15A88: hands its layout to the menu root (mode 7, and mode 0 again
 * when flag 0x40000 is set), then state 4. */
void func_08A12184(Screen15A88 *self)
{
    Work15A88 *work;

    work = self->work;
    func_08A22514(D_08A23FF0, 7, &work->layout, work->handles, work->texts, 0, 0xFFFFFFFF, 0);
    work = self->work;
    if ((work->flags & 0x40000) != 0) {
        func_08A1E894(D_08A23FF0, 0, &work->layout, work->handles, work->texts, 0);
        work = self->work;
    }
    work->state = 4;
}
