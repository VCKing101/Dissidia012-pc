#include "menu/auto_region3.h"

/* ObjB41E0: the value of kind 0..15. */
int func_08AB73A0(ObjB41E0 *self, int kind)
{
    int value;

    value = 0;
    if (kind == 0) {
        value = self->field_4C;
    }
    else if (kind == 1) {
        value = self->field_4E;
    }
    else if (kind == 2) {
        value = self->field_50;
    }
    else if (kind == 3) {
        value = self->field_52;
    }
    else if (kind == 4) {
        value = self->field_54;
    }
    else if (kind == 5) {
        value = self->field_56;
    }
    else if (kind == 6) {
        value = self->field_5A;
    }
    else if (kind == 7) {
        value = self->field_60;
    }
    else if (kind == 8) {
        value = self->field_5C;
    }
    else if (kind == 9) {
        value = self->field_5E;
    }
    else if (kind == 10) {
        value = self->field_58;
    }
    else if (kind == 0xB) {
        value = self->field_62;
    }
    else if (kind == 0xC) {
        value = self->field_64;
    }
    else if (kind == 0xD) {
        value = self->field_66;
    }
    else if (kind == 0xE) {
        value = self->field_68;
    }
    else if (kind == 0xF) {
        value = self->field_6A;
    }
    return value;
}
