#include "menu/auto_region2.h"

/* Panel61C40: opens on data with messages; keeps the cursor if it is in 0..3. */
void func_08A6082C(Panel61C40 *self, Data6FFC *data, void *messages, unsigned char flag)
{
    unsigned int value;
    int index;

    self->data = data;
    func_08A6108C(self, messages, flag);
    index = self->cursor.index;
    if (index < 0 || 3 < index) {
        index = 0;
    }
    func_088D46E4((MenuCursor *)&self->cursor, index, 0, 3, 0, -1);
    self->field_FC = 0;
    func_08A6171C(self);
    func_08A615C4(self);
    value = func_088729A0(0);
    self->field_124 = value;
}
