#include "battle/agent11_types.h"

extern "C" {

int func_08A0DEE8(BattleTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    if (self->state == 4) {
        self->flags[5] = 1;
    }
    return 0;
}

}
