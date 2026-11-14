#include "battle/agent23_types.h"

extern "C" {

extern int func_08891024(int side, int index);

/* Resets the children whose slot is in use. */
int func_08A02628(PairTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        if (func_08891024(self->side, i) == 1) {
            self->children[i]->reset();
        }
    }
    return 0;
}

/* Stops the children whose slot is in use. */
int func_08A026D8(PairTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        if (func_08891024(self->side, i) == 1) {
            self->children[i]->stop();
        }
    }
    return 0;
}

}
