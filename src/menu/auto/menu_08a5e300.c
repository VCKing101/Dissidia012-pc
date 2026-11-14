#include "menu/auto_region2.h"

extern const short D_08A5EF44[];
extern const unsigned short D_08A5EFD4[];

/* Panel5EED8: plays its layout script and hands its texts to its handles. */
void func_08A5E300(Panel5EED8 *self)
{
    func_088D3F00(0, self->handles, D_08A5EF44);
    func_088D44EC(self->handles, self->texts, D_08A5EFD4);
}
