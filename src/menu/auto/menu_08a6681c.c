#include "menu/auto_region2.h"

/* Panel68180: gives the selected entry the selected name and hands both on. */
void func_08A6681C(Panel68180 *self)
{
    int index;
    int name;
    List66 *list;

    index = func_08A6772C(self);
    name = func_08A67788(self);
    self->list->entries[index].field_06 = name;
    list = self->list;
    func_08A4E660(D_08A4FFF4, &list->entries[index], list->field_08, index, &list->names[name]);
}
