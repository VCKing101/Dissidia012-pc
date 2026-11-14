#include "battle/agent15_types.h"

struct NodeCheck {
    static bool flagged(SceneNode *node) { return (node->info->flags->flags & 0x8000) != 0; }
};

extern "C" bool func_08AD1228(SceneNode *self)
{
    if (NodeCheck::flagged(self) == 1 && self->slot_038() == 1) {
        return true;
    }
    return false;
}
