#include "battle/agent18_types.h"

extern "C" {

int func_08A0280C(ChildPairTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        self->children[i]->slot_060();
    }
    return 0;
}

}
