/* Sets the labels of slots 11 and 10 from the two current choices. */
#include "menu/agent24_types.h"

extern short D_08B04530[];
extern void func_08B02964(A24SlotOwner *self, int slot, int label);

void func_08B030F0(A24SlotOwner *self)
{
    int second = self->field_67C->field_658;
    func_08B02964(self, 0xB, D_08B04530[self->field_67C->field_654]);
    func_08B02964(self, 0xA, D_08B04530[second]);
}
