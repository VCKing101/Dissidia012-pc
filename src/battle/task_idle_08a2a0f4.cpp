#include "battle/agent11_types.h"

extern "C" {

bool func_08A2A0F4(BattleTask *self)
{
    if (self->isActive() == 0) {
        return false;
    }
    return self->state == -1;
}

}
