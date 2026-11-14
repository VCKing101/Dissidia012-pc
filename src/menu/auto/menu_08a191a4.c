#include "menu/auto_region1.h"

extern const short D_08A1ABC0[];
extern const short D_08A1ABD8[];
extern const short D_08A1AC00[];

/* Screen1AB08: plays D_08A1ABC0 and one of two scripts (by field_10F8),
 * hands its layout to the menu root and redraws its entries. */
void func_08A191A4(Screen1AB08 *self)
{
    Work1AB08 *work;

    func_088D3F00(&self->work->layout, self->work->handles, D_08A1ABC0);
    work = self->work;
    if (work->field_10F8 == 0) {
        func_088D3F00(&work->layout, work->handles, D_08A1ABD8);
    } else {
        func_088D3F00(&work->layout, work->handles, D_08A1AC00);
    }
    func_08A18E9C(self);
    work = self->work;
    func_08A1E6B4(D_08A23FF0, 0, &work->layout, work->handles, work->texts, &work->field_1708);
    func_08A18D28(self);
    func_08A18E08(self);
}
