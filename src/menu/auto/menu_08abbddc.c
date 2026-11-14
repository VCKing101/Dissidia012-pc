#include "menu/auto_region3.h"

/* ScreenC4560: releases field_B84 and frees field_B80 and field_B7C. */
void func_08ABBDDC(ScreenC4560 *self)
{
    WorkC4560 *work;

    work = self->work;
    if (work->field_B84 != 0) {
        func_088CB054(work->field_B84);
        func_088CAF90(self->work->field_B84);
        self->work->field_B84 = 0;
        work = self->work;
    }
    if (work->field_B80 != 0) {
        func_0885C538(self->heap, work->field_B80, 0);
        self->work->field_B80 = 0;
        work = self->work;
    }
    if (work->field_B7C != 0) {
        func_0885C538(self->heap, work->field_B7C, 0);
        self->work->field_B7C = 0;
    }
}
