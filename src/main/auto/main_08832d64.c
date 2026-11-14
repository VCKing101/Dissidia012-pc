#include "main/auto_region2.h"

/* func_08A51750 with mode 1, then sets flag 0x1 on target. */
void func_08832D64(void *self, Obj832D64 *target, int arg)
{
    func_08A51750(self, arg, 1);
    target->flags = target->flags | 1;
}
