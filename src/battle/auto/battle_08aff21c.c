#include "battle/auto_region5.h"

/* TimerFF1FC: reports its value (through a float) as kind 1. */
void func_08AFF21C(TimerFF1FC *self)
{
    func_08AFF33C(self, 1, (int)(float)self->value);
}
