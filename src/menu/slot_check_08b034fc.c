/* Looks the slot's id up and sets its bit 0 when the lookup's first word is
   0 (clears it otherwise). */
#include "menu/agent24_types.h"

extern void *D_08B01CF8;
extern void func_08AF9B88(void *queue, int id, int *out);

void func_08B034FC(A24SlotOwner *self, int index)
{
    int info[0x7FF];
    if (self->field_67C->field_660[index].id != 0) {
        unsigned short flags;
        func_08AF9B88(D_08B01CF8, self->field_67C->field_660[index].id, info);
        flags = self->field_67C->field_660[index].flags;
        if (info[0] == 0) {
            flags |= 1;
        } else {
            flags &= ~1;
        }
        self->field_67C->field_660[index].flags = flags;
    }
}
