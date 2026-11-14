#include "menu/auto_region2.h"

/* Screen4FFF4: Panel69A78 mode 3, keeps value, Panel63680 gets 4. */
int func_08A4DB78(Screen4FFF4 *self, int value)
{
    func_08A697B0(self->work->field_6FF8, 3, -1);
    self->work->field_719C = value;
    func_08A622FC(self->work->field_6FD8, 4);
    return 1;
}
