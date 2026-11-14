#include "main/auto_region2.h"

/* Runs the current step; when it reports 1, moves on and resets the next one. Returns 1 after the last. */
int func_088625D4(Obj8625D4 *self)
{
    int state;
    int done;

    done = 0;
    state = func_08861C64(self->steps[self->index]);
    if (state == 1) {
        state = self->index + 1;
        self->index = state;
        if (state == self->count) {
            done = 1;
        } else {
            func_08861C48(self->steps[state]);
        }
    }
    return done;
}
