#include "battle/agent11_types.h"

extern "C" {

int func_08A1AB80(BattleTask *self)
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
