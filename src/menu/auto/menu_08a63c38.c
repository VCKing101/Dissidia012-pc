#include "menu/auto_region2.h"

/* Page67EE0: in state 1 plays its scripts (func_08A63D34) and func_08A63D84. */
void func_08A63C38(Page67EE0 *self)
{
    if (self->field_10 == 1) {
        func_08A63D34(self);
        func_08A63D84(self);
    }
}
