#include "battle/agent11_types.h"

extern "C" {

bool func_08A1CCD8(BattleTask *self)
{
    if (self->isActive() == 0) {
        return false;
    }
    return self->state == 3;
}

}
