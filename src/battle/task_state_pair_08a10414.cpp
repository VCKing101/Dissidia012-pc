#include "battle/agent11_types.h"

extern "C" {

unsigned char func_08A10414(BattleTask *self)
{
    if (!self->isActive()) {
        return 0;
    }
    int result = 0;
    if (self->state == 1 || self->state == 2) {
        result = 1;
    }
    return result;
}

}
