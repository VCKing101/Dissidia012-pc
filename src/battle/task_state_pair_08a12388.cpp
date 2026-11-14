#include "battle/agent11_types.h"

extern "C" {

unsigned char func_08A12388(BattleTask *self)
{
    if (!self->isActive()) {
        return 0;
    }
    int result = 0;
    if (self->state == 8 || self->state == 12) {
        result = 1;
    }
    return result;
}

}
