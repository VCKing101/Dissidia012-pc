#include "battle/agent11_types.h"

extern "C" {

int func_089F453C(BattleTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    if (self->state == 3) {
        self->flags[1] = 1;
    }
    return 0;
}

}
