#include "menu/auto_region2.h"

extern const short D_08A6950C[];
extern const unsigned short D_08A69594[];

/* Panel694A8: plays its layout script and hands its texts to its handles. */
void func_08A68654(Panel694A8 *self)
{
    func_088D3F00(0, self->handles, D_08A6950C);
    func_088D44EC(self->handles, self->texts, D_08A69594);
}
