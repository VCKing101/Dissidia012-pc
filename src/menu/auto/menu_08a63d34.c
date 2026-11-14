#include "menu/auto_region2.h"

extern const unsigned short D_08A681D8[];

/* Page67EE0: plays its layout script and hands its texts to its handles. */
void func_08A63D34(Page67EE0 *self)
{
    func_088D3F00(0, self->handles, self->script);
    func_088D44EC(self->handles, self->texts, D_08A681D8);
}
