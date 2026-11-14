#include "battle/auto_region4.h"

/* PartC6098 release: frees the three handles and the object at field_0C. */
void func_08AC60BC(PartC6098 *self)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (self->handles[i] != 0) {
            func_0899BB1C(self->handles[i]);
        }
        self->handles[i] = 0;
    }
    func_08842138(self->field_0C);
}
