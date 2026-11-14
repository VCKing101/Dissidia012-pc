#include "battle/agent11_types.h"

extern "C" {

/* Marks the task (flag 0) while it is in any of states 0 to 6. */
int func_08A1F270(BattleTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    if (self->state == 0 || self->state == 1 || self->state == 2 || self->state == 3 || self->state == 4 || self->state == 5 || self->state == 6) {
        self->flags[0] = 1;
    }
    return 0;
}

}
