#include "menu/auto_region1.h"

extern const short D_08A09E52[];
extern const short D_08A09EC2[];
extern const unsigned short D_08A0A032[];
extern const short D_08A0A052[];
extern const unsigned short D_08A0A0A2[];
extern const short D_08A0A102[];
extern const short D_08A0A12A[];

/* Screen0B6A0: plays one of two sets of scripts, picked by field_3410 and
 * field_33E8. */
void func_089F4F88(Screen0B6A0 *self)
{
    Work0B6A0 *work;

    work = self->work;
    if ((work->field_3410 == 0) || (work->field_33E8 == 0)) {
        func_088D3F00(&work->layout, work->handles, D_08A09E52);
        func_088D3F00(&self->work->layout, self->work->handles, D_08A09EC2);
        func_088D44EC(self->work->handles, self->work->texts, D_08A0A032);
    } else {
        func_088D3F00(&work->layout, work->handles, D_08A0A052);
        func_088D44EC(self->work->handles, self->work->texts, D_08A0A0A2);
        func_088D3F00(&self->work->layout, self->work->handles, D_08A0A102);
        func_088D3F00(&self->work->layout, self->work->handles, D_08A0A12A);
    }
}
