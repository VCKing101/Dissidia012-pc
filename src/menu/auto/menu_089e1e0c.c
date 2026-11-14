#include "menu/auto_region1.h"

extern const unsigned short D_089E6B66[];
extern const short D_089E67B2[];

/* ScreenE75C4: plays the text script D_089E6B66 and the layout script
 * D_089E67B2. */
void func_089E1E0C(ScreenE75C4 *self)
{
    func_088D44EC(self->work->handles, self->work->texts, D_089E6B66);
    func_088D3F00(&self->work->layout, self->work->handles, D_089E67B2);
}
