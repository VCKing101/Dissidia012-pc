#include "menu/auto_region2.h"

extern const short D_08A6C7F4[];
extern const unsigned short D_08A6C9CC[];

/* Panel6C638: plays its layout script and hands its texts to its handles. */
void func_08A6B8C8(Panel6C638 *self)
{
    func_088D3F00(0, self->handles, D_08A6C7F4);
    func_088D44EC(self->handles, self->texts, D_08A6C9CC);
}
