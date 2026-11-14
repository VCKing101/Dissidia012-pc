#include "menu/auto_region2.h"

extern const unsigned short D_08A68218[];

/* Page68030: plays its layout script and hands its texts to its handles. */
void func_08A64FF4(Page68030 *self)
{
    func_088D3F00(0, self->handles, self->script);
    func_088D44EC(self->handles, self->texts, D_08A68218);
}
