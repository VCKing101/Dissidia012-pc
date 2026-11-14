#include "menu/auto_region3.h"

/* ScreenB01CF8: releases field_2EA0 and frees field_2E9C and field_2E98. */
void func_08AF4A94(ScreenB01CF8 *self)
{
    WorkB01CF8 *work;

    work = self->work;
    if (work->field_2EA0 != 0) {
        func_088CB054(work->field_2EA0);
        func_088CAF90(self->work->field_2EA0);
        self->work->field_2EA0 = 0;
        work = self->work;
    }
    if (work->field_2E9C != 0) {
        func_0885C538(self->heap, work->field_2E9C, 0);
        self->work->field_2E9C = 0;
        work = self->work;
    }
    if (work->field_2E98 != 0) {
        func_0885C538(self->heap, work->field_2E98, 0);
        self->work->field_2E98 = 0;
    }
}
