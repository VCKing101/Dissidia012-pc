#include "battle/agent11_types.h"

extern "C" {

int func_089FD9D4(BattleTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    if (self->state == 0 || self->state == 1) {
        self->flags[0] = 1;
    }
    return 0;
}

}
