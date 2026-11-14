/* Copies the five slot ids and their bit 0 into the message, then sends it. */
#include "menu/agent24_types.h"

extern void *D_08B01CF8;
extern void func_08AF34B0(void *queue, A24SlotMsg *message);

void func_08B02638(A24SlotOwner *self)
{
    int i;
    for (i = 0; i < 5; i++) {
        self->field_67C->field_674->field_4[i] = self->field_67C->field_660[i].id;
        if ((self->field_67C->field_660[i].flags & 1) != 0) {
            self->field_67C->field_674->field_18[i] |= 1;
        } else {
            self->field_67C->field_674->field_18[i] &= ~1;
        }
    }
    func_08AF34B0(D_08B01CF8, self->field_67C->field_674);
}
