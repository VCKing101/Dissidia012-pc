#include "battle/agent18_types.h"

extern "C" {

int func_08A02474(ChildPairTask *self)
{
    if (!self->isActive()) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        self->children[i]->slot_028();
    }
    return 0;
}

}
