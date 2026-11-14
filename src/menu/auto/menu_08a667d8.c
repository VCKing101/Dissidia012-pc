#include "menu/auto_region2.h"

/* Panel68180: opens Panel5DB80 on the selected entry. */
void func_08A667D8(Panel68180 *self)
{
    int index;

    index = func_08A6772C(self);
    func_08A4E63C(D_08A4FFF4, &self->list->entries[index]);
}
