#include "battle/agent11_types.h"

extern "C" {

unsigned char func_08A14E04(BattleTask *self)
{
    if (!self->isActive()) {
        return 0;
    }
    int result = 0;
    if (self->state == 4 || self->state == 8) {
        result = 1;
    }
    return result;
}

}
