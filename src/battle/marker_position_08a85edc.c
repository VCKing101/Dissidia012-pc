#include "battle/agent14_types.h"

extern Vec4 D_09AF84C0;
extern int func_08A85EBC(Marker *self);

/* The marker's position, or the shared origin while it is not placed. */
Vec4 *func_08A85EDC(Marker *self)
{
    if (func_08A85EBC(self) == 0) {
        return &D_09AF84C0;
    }
    if (func_08A85EBC(self) == 0) {
        return &D_09AF84C0;
    }
    return &self->position;
}
