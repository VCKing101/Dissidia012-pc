#include "battle/auto_region4.h"

/* NodeD2ABC: sets flag 4 when `flag` is 1 and moves the node to state 2. */
void func_08AD2ABC(NodeD2ABC *self, unsigned char flag)
{
    if (flag == 1) {
        self->flags = self->flags | 4;
    }
    self->state = 2;
}
