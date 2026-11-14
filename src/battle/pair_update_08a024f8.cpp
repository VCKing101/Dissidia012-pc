#include "battle/agent23_types.h"

extern "C" {

/* Updates both children. */
int func_08A024F8(PairTask *self, float x, float y)
{
    if (!self->isActive()) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        self->children[i]->update(x, y);
    }
    return 0;
}

}
