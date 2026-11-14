/* Shows icon `id` on slot part 11 - `index` when `on` is 1, else hides it. */
#include "menu/agent24_types.h"

extern short D_08B0453A[];
extern void func_088D9F5C(void *part, int id, int show);
extern void func_088D9CFC(void *part);

void func_08B03080(A24SlotOwner *self, unsigned char on, int index, int id)
{
    if (on == 1) {
        func_088D9F5C(self->field_67C->field_400[0xB - index], D_08B0453A[id], 1);
    } else {
        func_088D9CFC(self->field_67C->field_400[0xB - index]);
    }
}
