#include "menu/auto_region3.h"

/* View8E3B4: sets up field_18 and shows the twenty entries of field_354. */
void func_08A8BBC0(View8E3B4 *self)
{
    int count;
    Work8E3B4 *work;
    int i;
    Obj96D10 *list;

    list = &self->field_18;
    func_08A97750(list, self->field_00, self->work->field_318);
    count = func_08A96D4C(list, 0);
    self->work->field_2D8 = count;
    i = 0;
    work = self->work;
    do {
        func_08A8DFA4(self, i, work->field_354[i], 0);
        i = i + 1;
        work = self->work;
    } while (i < 0x14);
    func_08A97030(list, work->messages);
}
