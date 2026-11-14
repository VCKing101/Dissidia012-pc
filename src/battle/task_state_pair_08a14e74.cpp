#include "battle/agent11_types.h"

extern "C" {

unsigned char func_08A14E74(BattleTask *self)
{
    if (!self->isActive()) {
        return 0;
    }
    int result = 0;
    if (self->state == 7 || self->state == 11) {
        result = 1;
    }
    return result;
}

}
