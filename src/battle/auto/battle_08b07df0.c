#include "battle/auto_region5.h"

/* Part06984: notes whether D_08B05258 is in mode 2. */
void func_08B07DF0(Part06984 *self)
{
    self->field_3888 = func_08B05008(D_08B05258) == 2;
}
