#include "battle/auto_region5.h"

/* Sets flag 0x20 on the object at field_04, if any. */
int func_08B214DC(Part214DC *self)
{
    Node214DC *node;

    node = self->field_04;
    if (node != 0) {
        node->flags = node->flags | 0x20;
    }
    return 0;
}
