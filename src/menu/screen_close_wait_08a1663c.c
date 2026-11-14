#include "menu/agent29_types.h"

/* Screen181E0: once the object in D_089D69E8 is idle, closes it and moves
   to state 3 when it accepts. */
extern int func_08885938(void *object);
extern void func_08885988(void *object);
extern int func_08885608(void *object, char value);

void func_08A1663C(Screen181E0 *self)
{
    if (func_08885938(D_089D69E8) != 0) {
        return;
    }
    func_08885988(D_089D69E8);
    if (func_08885608(D_089D69E8, 1) == 0) {
        return;
    }
    WORK_INT(self->work, 0xC80) = 3;
}
