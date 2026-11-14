#include "battle/auto_region5.h"

/* TimerFF1FC: reports its value (through a float) as kind 4. */
void func_08AFF2AC(TimerFF1FC *self)
{
    func_08AFF33C(self, 4, (int)(float)self->value);
}
