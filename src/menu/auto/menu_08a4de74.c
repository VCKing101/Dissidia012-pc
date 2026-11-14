#include "menu/auto_region2.h"

/* Screen4FFF4: copies Panel68180's entries (0x160 bytes) and names (0x3FC)
 * out of the work buffer, then state 0x15. */
void func_08A4DE74(Screen4FFF4 *self)
{
    List66 *list;

    list = func_08A66AA0(self->work->field_6FEC);
    func_08935524(list->entries, self->work->field_6B20, 0x160);
    func_08935524(list->names, self->work->field_6B20 + 0x160, 0x3FC);
    func_08A4D07C(self, 0, 0);
    self->work->state = 0x15;
}
