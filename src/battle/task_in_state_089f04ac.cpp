#include "battle/agent11_types.h"

extern "C" {

bool func_089F04AC(BattleTask *self)
{
    if (self->isActive() == 0) {
        return false;
    }
    return self->state == 2;
}

}
