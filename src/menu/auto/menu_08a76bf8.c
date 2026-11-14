#include "menu/auto_region3.h"

/* Obj76BF8: shows or hides the node. */
void func_08A76BF8(Obj76BF8 *self, int shown)
{
    if (shown == 1) {
        func_088D3174(&self->node);
    }
    else {
        func_088D319C(&self->node);
    }
    self->shown = shown;
}
