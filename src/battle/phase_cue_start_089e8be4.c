#include "battle/agent28_types.h"

/* Starts the cue that belongs to the phase kind. */
extern int func_089E884C(PhaseTimer *timer, int which, void *table, void *cue);
extern char D_089E8D4C[];
extern char D_089E8D9C[];

int func_089E8BE4(PhaseTimer *timer)
{
    switch (timer->kind) {
    case 0:
        if (func_089E884C(timer, 1, D_089E8D4C, timer->cue) != 0) {
            return -1;
        }
        break;
    case 1:
        if (func_089E884C(timer, 0, D_089E8D4C, timer->cue) != 0) {
            return -2;
        }
        break;
    case 4:
    case 5:
        if (func_089E884C(timer, 2, D_089E8D9C, timer->cue) != 0) {
            return -3;
        }
        break;
    }
    return 0;
}
