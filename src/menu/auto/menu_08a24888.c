#include "menu/auto_region1.h"

extern const short D_08A28718[];

/* Screen28560: plays D_08A28718 and redraws its parts. */
void func_08A24888(Screen28560 *self)
{
    func_088D3F00(&self->work->layout, self->work->handles, D_08A28718);
    func_08A243C4(self);
    func_08A24304(self);
    func_08A24524(self);
    func_08A247AC(self);
}
