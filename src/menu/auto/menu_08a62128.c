#include "menu/auto_region2.h"

/* Panel63680: opens on data with messages. */
void func_08A62128(Panel63680 *self, void *data, void *messages)
{
    int time;

    time = func_088729A0(0);
    self->field_D88 = time;
    self->field_E00 = data;
    func_08A625C8(self, messages);
    func_08A6324C(self);
}
