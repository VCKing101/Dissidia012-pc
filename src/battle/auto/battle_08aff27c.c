#include "battle/auto_region5.h"

/* TimerFF1FC: reports its value (through a float) as kind 3. */
void func_08AFF27C(TimerFF1FC *self)
{
    func_08AFF33C(self, 3, (int)(float)self->value);
}
