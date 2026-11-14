/* Picks a slot 0-3 other than the one after the current slot, or that one
 * when the count is full. */
struct SlotParts {
    unsigned char pad_00[4];
    int current;
    unsigned char pad_08[0x18];
    int limit;
    int count;
};

struct SlotOwner {
    unsigned char pad_00[0x14];
    SlotParts *parts;
};

extern int D_09AF8B00;

struct SlotRand {
    static int next()
    {
        return (D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    }
};

extern "C" int func_08A43B50(SlotOwner *self)
{
    SlotParts *parts = self->parts;
    int slot;
    if (parts->count == parts->limit) {
        slot = parts->current + 1;
    } else {
        do {
            slot = SlotRand::next() % 4;
        } while (slot == self->parts->current + 1);
    }
    return slot;
}
