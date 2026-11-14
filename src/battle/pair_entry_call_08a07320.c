typedef struct PairEntry {
    char data[0x34];
} PairEntry;

typedef struct PairOwner {
    unsigned char active;
    char pad_01[0x163];
    PairEntry entries[2];
} PairOwner;

extern int func_08A0641C(PairEntry *entry);

int func_08A07320(PairOwner *self, unsigned int index)
{
    if (!self->active) {
        return -1;
    }
    if (index >= 2) {
        return -2;
    }
    if (func_08A0641C(&self->entries[index]) != 0) {
        return -3;
    }
    return 0;
}
