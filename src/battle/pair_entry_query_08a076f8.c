typedef struct PairEntryB0 {
    char data[0xB0];
} PairEntryB0;

typedef struct PairOwnerB0 {
    unsigned char active;
    char pad_001[0x223];
    PairEntryB0 entries[2];
} PairOwnerB0;

extern int func_08A03F54(PairEntryB0 *entry);

int func_08A076F8(PairOwnerB0 *self, unsigned int index)
{
    PairEntryB0 *entries;
    if (!self->active) {
        return 0;
    }
    if (index >= 2) {
        return 0;
    }
    entries = self->entries;
    return func_08A03F54(&entries[index]);
}
