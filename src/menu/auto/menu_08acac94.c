#include "menu/auto_region3.h"

extern const unsigned short D_08AD96BC[];
extern const unsigned short D_08AD972C[];
extern const unsigned short D_08AD97A4[];
extern const unsigned short D_08AD9824[];

/* ScreenDB30: plays one of two script pairs by field_F28. */
void func_08ACAC94(ScreenDB30 *self)
{
    WorkDB30 *work;

    work = self->work;
    if (work->field_F28 == 0) {
        func_088D3EA0(&work->layout, work->handles, D_08AD96BC);
        func_088D436C(self->work->handles, self->work->texts, D_08AD972C);
    }
    else {
        func_088D3EA0(&work->layout, work->handles, D_08AD97A4);
        func_088D436C(self->work->handles, self->work->texts, D_08AD9824);
    }
}
