#include "menu/auto_region2.h"

/* Panel5DB80: acts on the selected cell, or confirms the entry and closes. */
void func_08A5D95C(Panel5DB80 *self)
{
    int index;

    index = func_08A5D654(self);
    if (index < 16) {
        func_08A5D754(self, index);
    } else {
        func_08A5D9D4(self, self->field_644);
        func_08A5CE08(self);
    }
}
