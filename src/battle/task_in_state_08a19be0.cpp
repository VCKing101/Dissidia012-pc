#include "battle/agent11_types.h"

extern "C" {

bool func_08A19BE0(BattleTask *self, unsigned int index)
{
    if (self->isActive() == 0) {
        return false;
    }
    if (index >= 2) {
        return false;
    }
    return self->state == 2;
}

}
