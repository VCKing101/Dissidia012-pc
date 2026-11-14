#include "menu/auto_region2.h"

/* Screen4FFF4: copies Panel68180's entries and names into the work buffer. */
void func_08A4DF14(Screen4FFF4 *self)
{
    List66 *list;

    list = func_08A66AA0(self->work->field_6FEC);
    func_08935524(self->work->field_6B20, list->entries, 0x160);
    func_08935524(self->work->field_6B20 + 0x160, list->names, 0x3FC);
}
