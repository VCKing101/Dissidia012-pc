#include "menu/agent36_types.h"

/* Picker: the next mode (forward or back, wrapping over 13) that has an
 * entry; the current one when there is none or fewer than two modes. */
int func_08A31F34(Picker36 *self)
{
    PickWork36 *work = self->work;
    int mode = work->mode;
    int next;
    int i;

    if (work->mode_count < 2) {
        return mode;
    }
    next = mode;
    if (work->forward != 0) {
        for (i = 0; i < 13; i++) {
            next++;
            if (next >= 13) {
                next = 0;
            }
            if (work->modes[next] != 0xFFFF) {
                return next;
            }
        }
    } else {
        for (i = 0; i < 13; i++) {
            next--;
            if (next < 0) {
                next = 12;
            }
            if (work->modes[next] != 0xFFFF) {
                return next;
            }
        }
    }
    return mode;
}
