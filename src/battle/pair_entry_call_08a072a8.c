typedef struct PairEntryB0 {
    char data[0xB0];
} PairEntryB0;

typedef struct PairOwnerB0 {
    unsigned char active;
    char pad_001[0x223];
    PairEntryB0 entries[2];
} PairOwnerB0;

extern int func_08A03DA4(PairEntryB0 *entry);

int func_08A072A8(PairOwnerB0 *self, unsigned int index)
{
    if (!self->active) {
        return -1;
    }
    if (index >= 2) {
        return -2;
    }
    if (func_08A03DA4(&self->entries[index]) != 0) {
        return -3;
    }
    return 0;
}
