#include "battle/agent18_types.h"

extern "C" {

int func_08A02BD8(ChildPairTask *self, unsigned int side)
{
    if (!self->isActive()) {
        return 0;
    }
    if (side >= 2) {
        return 0;
    }
    BattleObject **children = self->children;
    int other = self->field_10 != side;
    return children[other]->slot_090();
}

}
