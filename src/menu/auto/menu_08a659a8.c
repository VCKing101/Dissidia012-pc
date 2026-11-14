#include "menu/auto_region2.h"

/* Page680D8: hands its entry and its name to the save screen. */
void func_08A659A8(Page680D8 *self)
{
    unsigned short index;

    index = self->entry->field_06;
    func_08A4DE14(D_08A4FFF4);
    func_08A4E660(D_08A4FFF4, self->entry, self->field_94, self->field_90, &self->names[index]);
}
