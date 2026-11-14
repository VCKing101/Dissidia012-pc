#include "battle/agent15_types.h"

extern "C" {

int func_08AB6ADC(ActionHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    self->slot_168();
    self->state = 1;
    return 0;
}

}
