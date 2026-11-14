#include "battle/agent14_types.h"

extern int func_08A86C60(LinkSlot *slot);

int func_08A8707C(LinkSet *self, unsigned int index)
{
    LinkSlot *slots;

    if (self->active == 0) {
        return 0;
    }
    if (index >= 3) {
        return 0;
    }
    slots = self->slots;
    return func_08A86C60(&slots[index]);
}
