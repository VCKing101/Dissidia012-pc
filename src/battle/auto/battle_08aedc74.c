#include "battle/auto_region4.h"

typedef struct StateEDC74 {
    char pad_00[8];
    int field_08;
} StateEDC74;

/* Sets field_08 to 2. */
void func_08AEDC74(StateEDC74 *self)
{
    self->field_08 = 2;
}
