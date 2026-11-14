#include "battle/agent15_types.h"

extern "C" {

int func_08AB6A40(ActionHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    return self->value;
}

}
