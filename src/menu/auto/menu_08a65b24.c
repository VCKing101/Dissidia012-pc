#include "menu/auto_region2.h"

extern const unsigned short D_08A68230[];

/* Page680D8: plays its layout script and hands its texts to its handles. */
void func_08A65B24(Page680D8 *self)
{
    func_088D3F00(0, self->handles, self->script);
    func_088D44EC(self->handles, self->texts, D_08A68230);
}
