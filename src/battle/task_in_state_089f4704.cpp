#include "battle/agent11_types.h"

extern "C" {

bool func_089F4704(BattleTask *self)
{
    if (self->isActive() == 0) {
        return false;
    }
    return self->state == 3;
}

}
