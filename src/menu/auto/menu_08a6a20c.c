#include "menu/auto_region2.h"

extern const short D_08A6B260[];
extern const unsigned short D_08A6B348[];

/* Panel6B170: plays its layout script and hands its text to its handles. */
void func_08A6A20C(Panel6B170 *self)
{
    func_088D3F00(0, self->handles, D_08A6B260);
    func_088D44EC(self->handles, self->texts, D_08A6B348);
}
