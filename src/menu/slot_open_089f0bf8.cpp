/* Returns 1 when the slot is unlocked and none of the three blocking checks applies. */
typedef struct SlotFlags {
    char pad_0000[0x32F8];
    unsigned int field_32F8;
} SlotFlags;

typedef struct SlotOwner {
    char pad_000[0x14C];
    SlotFlags *field_14C;
} SlotOwner;

extern "C" {
int func_089FC218(SlotOwner *self, int slot);
int func_089FC478(SlotOwner *self, int slot);
int func_089F0B98(SlotOwner *self, int slot);

bool func_089F0BF8(SlotOwner *self, signed char slot)
{
    if ((self->field_14C->field_32F8 & (1 << (slot - 1))) == 0) {
        return false;
    }
    if (func_089FC218(self, slot) != 0) {
        return false;
    }
    if (func_089FC478(self, slot) != 0) {
        return false;
    }
    if (func_089F0B98(self, slot) != 0) {
        return false;
    }
    return true;
}
}
