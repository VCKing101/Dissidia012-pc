#include "battle/auto_region4.h"

typedef struct FlagsC5BF8 {
    char pad_00[0xC];
    char field_0C;
    char field_0D;
} FlagsC5BF8;

/* Sets field_0C and field_0D. */
void func_08AC5BF8(FlagsC5BF8 *self)
{
    self->field_0C = 1;
    self->field_0D = 1;
}
