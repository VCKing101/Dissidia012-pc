#include "battle/agent15_types.h"

extern "C" void func_08AD046C(SceneNodeList *self)
{
    SceneNode *node = 0;
    while ((node = SceneNodeList::nextOf(self, node)) != 0) {
        node->slot_018(0);
    }
}
