#include "battle/auto_region4.h"

typedef struct StateC4934 {
    char pad_00[4];
    int field_04;
} StateC4934;

void func_08AC485C(StateC4934 *self);

/* Calls func_08AC485C, then sets field_04 to 5. */
void func_08AC4934(StateC4934 *self)
{
    func_08AC485C(self);
    self->field_04 = 5;
}
