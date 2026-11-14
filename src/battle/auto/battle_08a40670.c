#include "battle/auto_region3.h"

typedef struct Triple40670 {
    int field_00;
    int field_04;
    int field_08;
} Triple40670;

/* Clears field_04 and field_00 and sets field_08 to -1. */
void func_08A40670(Triple40670 *self)
{
    self->field_04 = 0;
    self->field_00 = 0;
    self->field_08 = -1;
}
