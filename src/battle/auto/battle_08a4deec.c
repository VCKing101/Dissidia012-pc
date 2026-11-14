#include "battle/auto_region3.h"

typedef struct Phase4DEEC {
    char pad_00[8];
    int field_08;
} Phase4DEEC;

int func_08A406C8(Phase4DEEC *self);
void func_08A4E178(Phase4DEEC *self);
void func_08A4E1D8(Phase4DEEC *self);

/* Calls func_08A4E178 when func_08A406C8 reports 1, then func_08A4E1D8 while
 * field_08 is 0. */
void func_08A4DEEC(Phase4DEEC *self)
{
    if (func_08A406C8(self) == 1) {
        func_08A4E178(self);
    }
    if (self->field_08 == 0) {
        func_08A4E1D8(self);
    }
}
