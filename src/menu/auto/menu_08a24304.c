#include "menu/auto_region1.h"

extern const unsigned short D_08A28850[];

/* Screen28560: plays the text script D_08A28850. */
void func_08A24304(Screen28560 *self)
{
    func_088D44EC(self->work->handles, self->work->texts, D_08A28850);
}
