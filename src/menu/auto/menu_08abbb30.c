#include "menu/auto_region3.h"

/* ScreenC4560: frees the blocks field_B60..field_B70. */
void func_08ABBB30(ScreenC4560 *self)
{
    WorkC4560 *work;

    work = self->work;
    if (work->field_B70 != 0) {
        func_08A49684(work->field_B70);
        work = self->work;
    }
    if (work->field_B6C != 0) {
        func_0885C538(self->heap, work->field_B6C, 0);
        self->work->field_B6C = 0;
        self->work->field_B70 = 0;
        work = self->work;
    }
    if (work->field_B68 != 0) {
        func_0885C538(self->heap, work->field_B68, 0);
        self->work->field_B68 = 0;
        work = self->work;
    }
    if (work->field_B64 != 0) {
        func_08A2958C();
        func_0885C538(self->heap, self->work->field_B64, 0);
        self->work->field_B64 = 0;
        work = self->work;
    }
    if (work->field_B60 != 0) {
        func_0885C538(self->heap, work->field_B60, 0);
        self->work->field_B60 = 0;
    }
}
