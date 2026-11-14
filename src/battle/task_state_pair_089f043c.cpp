#include "battle/agent11_types.h"

extern "C" {

unsigned char func_089F043C(BattleTask *self)
{
    if (!self->isActive()) {
        return 0;
    }
    int result = 0;
    if (self->state == 7 || self->state == 10) {
        result = 1;
    }
    return result;
}

}
