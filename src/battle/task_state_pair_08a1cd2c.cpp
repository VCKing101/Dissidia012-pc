#include "battle/agent11_types.h"

extern "C" {

unsigned char func_08A1CD2C(BattleTask *self)
{
    if (!self->isActive()) {
        return 0;
    }
    int result = 0;
    if (self->state == 5 || self->state == 9) {
        result = 1;
    }
    return result;
}

}
