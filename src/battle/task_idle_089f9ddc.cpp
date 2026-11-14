#include "battle/agent11_types.h"

extern "C" {

bool func_089F9DDC(BattleTask *self)
{
    if (self->isActive() == 0) {
        return false;
    }
    return self->state == -1;
}

}
