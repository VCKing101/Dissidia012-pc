#include "menu/agent29_types.h"

/* Screen0B6A0: starts the pull of mode `mode` with `value`, state 0x10. */
typedef struct PullState29 {
    int field_00;
    int field_04;
    int mode;        /* +0x08 */
    int field_0C;
    void *sound;     /* +0x10 */
} PullState29;

extern PullState29 D_09B6DA40;

void func_089FA3BC(Screen0B6A0 *self, int mode, int value)
{
    PullState29 *pull = &D_09B6DA40;
    pull->mode = mode;
    WORK_INT(self->work, 0x33C8C) = value;
    self->work->state = 0x10;
}
