#include "battle/auto_region4.h"

/* PartF3D40: copies the handle's matrix and value (func_088DA0A8) to the
 * object at field_08 and updates it. */
void func_08AF3D6C(PartF3D40 *self)
{
    char matrix[64];
    char value[4];

    if (self->field_04 != 0) {
        func_088DA0A8(self->field_04, 0, matrix, value);
        func_0887B778(self->field_08, matrix);
        func_0887B828(self->field_08, value);
        func_0887B958(self->field_08);
    }
}
