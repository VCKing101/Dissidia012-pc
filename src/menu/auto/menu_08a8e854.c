#include "menu/auto_region3.h"

/* View8E3B4: picks record index for slot value and redraws. */
void func_08A8E854(View8E3B4 *self, int index, int value)
{
    int ok;
    Work8E3B4 *work;

    ok = func_08A94724(&self->field_0C, &self->work->records[index]);
    if (ok == 1) {
        self->work->field_2BC[index] = value;
        work = self->work;
        func_08A95A38(&self->field_0C, &work->records[index], work->layout, work->handles, 1);
        func_08A8D2BC(self, -1);
        func_08A8CD90(self, 1, -1);
        func_08A8E0C0(self, 1);
    }
}
