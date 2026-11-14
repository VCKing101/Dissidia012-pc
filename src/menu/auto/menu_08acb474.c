#include "menu/auto_region3.h"

/* ScreenDB30: marks the cursor row (0x21) and field_DB10's row (0x22). */
void func_08ACB474(ScreenDB30 *self)
{
    int index;

    func_08ACB3AC(self, self->work->cursor.index, 0x21);
    index = self->work->field_DB10;
    if (index != -1) {
        func_08ACB3AC(self, index, 0x22);
    }
}
