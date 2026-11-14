#include "menu/agent29_types.h"

/* Screen15A88: sorts the board's pieces (+0xCE4 of them, 0x1E bytes each
   from +0xCE8) into the list of those with flag 2 (+0xF00, count +0xF40)
   and the rest (+0xF20, count +0xF42); unused places hold -1. */
void func_08A13C6C(Screen15A88 *self)
{
    int marked = 0;
    int other = 0;
    int i;

    for (i = 0; i < 0x10; i++) {
        (&WORK_SHORT(self->work, 0xF00))[i] = -1;
        (&WORK_SHORT(self->work, 0xF20))[i] = -1;
    }
    WORK_SHORT(self->work, 0xF40) = 0;
    WORK_SHORT(self->work, 0xF42) = 0;
    for (i = 0; i < WORK_BYTE(self->work, 0xCE4); i++) {
        if ((&WORK_BYTE(self->work, 0xCE8))[i * 0x1E] & 2) {
            (&WORK_SHORT(self->work, 0xF00))[marked] = i;
            marked++;
        } else {
            (&WORK_SHORT(self->work, 0xF20))[other] = i;
            other++;
        }
    }
    WORK_SHORT(self->work, 0xF40) = marked;
    WORK_SHORT(self->work, 0xF42) = other;
}
