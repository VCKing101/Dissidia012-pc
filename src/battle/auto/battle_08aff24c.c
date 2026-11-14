#include "battle/auto_region5.h"

/* TimerFF1FC: reports its value (through a float) as kind 2. */
void func_08AFF24C(TimerFF1FC *self)
{
    func_08AFF33C(self, 2, (int)(float)self->value);
}
