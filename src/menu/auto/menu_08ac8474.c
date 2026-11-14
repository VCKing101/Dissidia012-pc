#include "menu/auto_region3.h"

/* PanelC18: plays the scripts it was given and its text script. */
void func_08AC8474(PanelC18 *self)
{
    WorkC18 *work;
    MenuModel **handles;

    work = self->work;
    handles = work->handles;
    if (work->field_BD8 != 0) {
        func_088D3F00(work->layout, handles, work->field_BD8);
        work = self->work;
    }
    if (work->field_BDC != 0) {
        func_088D3F00(work->layout, handles, work->field_BDC);
        work = self->work;
    }
    if (work->field_BE0 != 0) {
        func_088D44EC(handles, work->texts, work->field_BE0);
        work = self->work;
    }
    func_088D44EC(handles, work->texts, work->field_BB0);
}
