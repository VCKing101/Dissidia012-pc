#include "battle/agent18_types.h"

extern "C" {

extern int func_0883739C(int target);

int func_08AB32B8(FighterObject *self)
{
    if (self == 0) {
        return 0;
    }
    int target = self->slot_130();
    if (target == 0) {
        return 0;
    }
    if (func_0883739C(target) == 0) {
        return 0;
    }
    return target;
}

}
