#include "battle/auto_region5.h"

/* Part49AAC: hands two values from D_089D712C to field_25C. */
void func_08B4A070(Part49AAC *self)
{
    int value;
    int id;

    func_0892D2C0(self->field_25C, self->field_00, 2, D_089D712C->field_10CC8);
    id = self->field_00;
    value = func_089203BC(D_089D712C->field_3F548);
    func_0892D2DC(self->field_25C, id, 2, value);
}
