#include "menu/agent20_types.h"

/* Moves the cursor to the current item's slot; same group: flag the direction instead of switching. */
typedef struct A20Item {
    char pad_0[0x4];
    short field_04;
} A20Item;

typedef struct A20Slot {
    char pad_0[0x2];
    signed char field_02;
    char pad_3[0x3];
    short field_06;
} A20Slot;

extern A20Item *func_089F0AE4(A20EntryOwner *self);
extern A20Slot *func_089FCA28(A20EntryOwner *self, unsigned short slot);
extern void func_089F2578(A20EntryOwner *self, int slot);
extern int func_089FE4CC(A20EntryOwner *self);

void func_089F25F8(A20EntryOwner *self)
{
    A20Item *item = func_089F0AE4(self);
    A20Slot *from = func_089FCA28(self, self->field_14C->field_272E);
    A20Slot *to = func_089FCA28(self, item->field_04);
    if (from->field_02 != to->field_02) {
        func_089F2578(self, item->field_04);
    } else if (from->field_06 < to->field_06) {
        self->field_14C->field_2724 |= 0x200000;
    } else {
        self->field_14C->field_2724 |= 0x800000;
    }
    self->field_14C->field_272E = item->field_04;
    if (func_089FE4CC(self) != 0) {
        self->field_14C->field_3280 |= 0x200000;
    }
}
