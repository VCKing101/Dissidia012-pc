typedef struct Entry18 {
    char data[0x18];
} Entry18;

typedef struct EntryTable18 {
    unsigned char active;
    char pad_01[0x37];
    unsigned int count;
    Entry18 *entries;
} EntryTable18;

/* Entry number index, 0 past the end or for an inactive table. */
Entry18 *func_08ABC420(EntryTable18 *self, unsigned int index)
{
    if (self->active == 0) {
        return 0;
    }
    if (index >= self->count) {
        return 0;
    }
    return &self->entries[index];
}
