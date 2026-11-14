#include "menu/auto_region2.h"

extern const short D_08A61CA8[];
extern const unsigned short D_08A61D78[];

/* Panel61C40: plays its layout script and hands its texts to its handles. */
void func_08A60BEC(Panel61C40 *self)
{
    func_088D3F00(0, self->handles, D_08A61CA8);
    func_088D44EC(self->handles, self->texts, D_08A61D78);
}
