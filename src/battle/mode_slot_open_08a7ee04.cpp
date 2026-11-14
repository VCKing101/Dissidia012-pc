/* True when the slot the current mode uses is not at its limit (false
 * when closed or out of range). */
struct ModeInfo {
    int slot;
    int pad_04[2];
};

struct ModeSlot {
    int value;
    int limit;
    unsigned char pad_08[0xC];
};

struct ModeSlots {
    ModeSlot items[1];

    static ModeSlot *at(ModeSlots *slots, int index) { ModeSlot *items = slots->items; return &items[index]; }
};

struct ModeTable {
    unsigned char open;
    unsigned char pad_01[0xF];
    unsigned int mode;
    unsigned char pad_14[0x14];
    ModeSlots slots;
};

extern ModeInfo D_08A7FFE0[7];

extern "C" int func_08A7EE04(ModeTable *self)
{
    if (!self->open) {
        return 0;
    }
    if (self->mode >= 7) {
        return 0;
    }
    ModeSlot *slot = ModeSlots::at(&self->slots, D_08A7FFE0[self->mode].slot);
    return slot->value != slot->limit;
}
