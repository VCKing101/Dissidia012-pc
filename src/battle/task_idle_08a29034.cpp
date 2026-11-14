#include "battle/agent11_types.h"

extern "C" {

bool func_08A29034(BattleTask *self)
{
    if (self->isActive() == 0) {
        return false;
    }
    return self->state == -1;
}

}
