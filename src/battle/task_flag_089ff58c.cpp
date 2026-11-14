#include "battle/agent11_types.h"

extern "C" {

int func_089FF58C(BattleTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    if (self->state == 0 || self->state == 1 || self->state == 2 || self->state == 3) {
        self->flags[0] = 1;
    }
    return 0;
}

}
