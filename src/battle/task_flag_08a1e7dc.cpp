#include "battle/agent11_types.h"

extern "C" {

int func_08A1E7DC(BattleTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    if (self->state == 0 || self->state == 1 || self->state == 2 || self->state == 3 || self->state == 4 || self->state == 5) {
        self->flags[0] = 1;
    }
    return 0;
}

}
