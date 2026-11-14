#include "battle/agent11_types.h"

extern "C" {

int func_089FA9B0(BattleTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    if (self->state == 0) {
        self->flags[0] = 1;
    }
    return 0;
}

}
