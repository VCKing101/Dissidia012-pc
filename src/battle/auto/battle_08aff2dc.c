#include "battle/auto_region5.h"

/* TimerFF1FC: reports its value (through a float) as kind 5. */
void func_08AFF2DC(TimerFF1FC *self)
{
    func_08AFF33C(self, 5, (int)(float)self->value);
}
