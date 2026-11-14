#include "menu/auto_region2.h"

/* Panel61C40: prints its data's field_10 into text 7. */
void func_08A613E8(Panel61C40 *self)
{
    void *text;
    unsigned short buffer[0x80];

    func_089291E4(buffer, 0x80, self->data->field_10, 0x79, 0x28);
    if (self->texts[7] != 0) {
        func_0887BA5C(self->texts[7]);
        self->texts[7] = 0;
    }
    text = func_0887D868(D_089D6998, 0, buffer, -1, 0, 0, 0, 0);
    self->texts[7] = text;
}
