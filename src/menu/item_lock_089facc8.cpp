/* Returns 0x100 when the item is not unlocked for the current rank, unless its group allows it. */
struct LockItem {
    char pad_00[0xD];
    signed char field_0D;
    char pad_0E[0x18 - 0xE];
    unsigned int field_18;

    static unsigned char unlockedAt(LockItem *item, int rank)
    {
        int unlocked = 0;
        if (rank > 0 && (item->field_18 & (1 << (rank - 1))) != 0) {
            unlocked = 1;
        }
        return unlocked;
    }
};

struct LockState {
    char pad_0000[0x328C];
    int field_328C;
    char pad_3290[0x32F4 - 0x3290];
    unsigned int field_32F4;
};

struct LockOwner {
    char pad_000[0x14C];
    LockState *field_14C;
};

extern "C" LockItem *func_088C0F8C(int id);

extern "C" {
unsigned short func_089FACC8(LockOwner *self, int id)
{
    unsigned short result = 0;
    LockItem *item = func_088C0F8C(id);
    if (item != 0) {
        if (!LockItem::unlockedAt(item, self->field_14C->field_328C)) {
            result = 0x100;
        }
        if ((self->field_14C->field_32F4 & (1 << item->field_0D)) != 0) {
            result = result & ~0x100;
        }
    }
    return result;
}
}
