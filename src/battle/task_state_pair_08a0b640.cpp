#include "battle/agent11_types.h"

extern "C" {

unsigned char func_08A0B640(BattleTask *self)
{
    if (!self->isActive()) {
        return 0;
    }
    int result = 0;
    if (self->state == 2 || self->state == 3) {
        result = 1;
    }
    return result;
}

}
