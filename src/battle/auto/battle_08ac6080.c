#include "battle/auto_region4.h"

typedef struct FlagC6080 {
    char pad_00[0xC];
    char field_0C;
} FlagC6080;

/* Clears field_0C. */
void func_08AC6080(FlagC6080 *self)
{
    self->field_0C = 0;
}
