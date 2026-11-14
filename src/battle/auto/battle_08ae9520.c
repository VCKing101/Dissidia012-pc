#include "battle/auto_region4.h"

typedef struct MotionE9520 {
    unsigned int flags;
    char pad_04[0x10];
    unsigned int field_14;
    unsigned int field_18;
} MotionE9520;

/* Node9520: fills the motion (field_14 = 0, field_18 = minus the manager's
 * field_08, flag 0x100) and sets flag 4. */
int func_08AE9520(Node9520 *self, MotionE9520 *motion)
{
    int value;

    value = func_08AE3C70();
    motion->field_14 = 0;
    motion->field_18 = -value;
    motion->flags = motion->flags | 0x100;
    self->flags = self->flags | 4;
    return 1;
}
