#include "battle/auto_region4.h"

typedef struct ValueCBCB0 {
    char pad_00[0x58];
    int field_58;
} ValueCBCB0;

/* Clears field_58. */
void func_08ACBCB0(ValueCBCB0 *self)
{
    self->field_58 = 0;
}
