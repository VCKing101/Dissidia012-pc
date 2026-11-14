#include "battle/agent18_types.h"

extern "C" {

int func_08A02890(ChildPairArgTask *self, int arg)
{
    if (!self->isActive()) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        self->children[i]->slot_068(arg);
    }
    return 0;
}

}
