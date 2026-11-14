#include "main/auto_region1.h"

/* Stores a duration and the rate that covers total in that time. */
void func_0880611C(float total, float duration, Chara *self)
{
    self->duration = duration;
    self->rate = total / duration;
}
