#include "menu/auto_region1.h"

extern const unsigned short D_08A11B2E[];
extern const unsigned short D_08A11B3E[];

/* Screen11510: plays two scripts, runs func_08A0E070 when flag 1 is set,
 * calls func_0887B944 on texts 0x4C..0x57, clears fields 30..38 of handles
 * 0x6A..0x6F and plays motions on handles 0x2C and 0x67. */
void func_08A0CE68(Screen11510 *self)
{
    Work11510 *work;
    MenuModel *model;
    int i;

    func_088D3EA0(&self->work->layout, self->work->handles, D_08A11B2E);
    func_088D3EA0(&self->work->layout, self->work->handles, D_08A11B3E);
    work = self->work;
    if ((work->flags & 1) != 0) {
        func_08A0E070(self);
        work = self->work;
    }
    i = 0x4C;
    do {
        func_0887B944(work->texts[i]);
        i++;
        work = self->work;
    } while (i < 0x58);
    i = 0x6A;
    do {
        model = work->handles[i];
        i++;
        model->field_34 = 0;
        model->field_30 = 0;
        model->field_38 = 0;
        work = self->work;
    } while (i < 0x70);
    func_088D9F5C(work->handles[0x2C], 7, 1);
    func_088D9F5C(self->work->handles[0x67], 0x16, 1);
}
