#include "battle/agent11_types.h"

extern "C" {

int func_08A14CCC(BattleTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    if (self->state == 4 || self->state == 8) {
        self->flags[1] = 1;
    }
    return 0;
}

}
