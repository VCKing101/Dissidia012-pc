#include "menu/auto_region3.h"

/* ObjB41E0: sets row index of the three rows and field_A8. */
void func_08AB6D04(ObjB41E0 *self, int index, int arg1, int arg2, int arg3, int arg4)
{
    self->field_6C[index] = arg1;
    self->field_80[index] = arg2;
    self->field_94[index] = arg3;
    self->field_A8 = arg4;
}
