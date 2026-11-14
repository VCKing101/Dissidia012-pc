#include "main/auto_region4.h"

/* Obj8F4C4C: takes the six id bytes from func_089C5918 when they changed. */
void func_088F4C4C(Obj8F4C4C *self)
{
    int differ;
    int value;
    unsigned char id[8];

    func_089C5918(id);
    differ = func_08935490(self->id, id, 6);
    if (differ != 0) {
        func_08935524(self->id, id, 6);
        value = func_088F5D40();
        self->field_0C = value;
    }
}
