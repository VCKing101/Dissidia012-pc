#include "battle/auto_region5.h"

/* TimerFF1FC: reports its value (through a float) as kind 6. */
void func_08AFF30C(TimerFF1FC *self)
{
    func_08AFF33C(self, 6, (int)(float)self->value);
}
