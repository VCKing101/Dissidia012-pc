#include "battle/auto_region5.h"

/* Part0E63C step: once both Part0F410 have finished (or field_00 or
 * field_10 reports done), finishes them and starts the next entry; returns
 * 1 when there is none. */
unsigned int func_08B0E724(Part0E63C *self)
{
    int done;
    int first;
    int second;
    int next;

    done = func_08B10BAC(self->field_10);
    first = func_08B0F32C(self->field_04);
    second = func_08B0F32C(self->field_08);
    next = 0;
    if (self->field_20 != 0) {
        next = func_08B0EA68(self->field_00);
    }
    func_08B0FD08(self->field_0C);
    if (((first == 1 && second == 1) || next == 1) || done == 1) {
        func_08B0F410(self->field_04);
        func_08B0F410(self->field_08);
        if (self->field_20 == 0) {
            return 1;
        }
        func_08B0E804(self);
    }
    return 0;
}
