typedef struct KindEntry {
    signed char low : 4;
    signed char high : 4;
    char pad_01[0x7];
} KindEntry;

typedef struct KindTable {
    char pad_00[0x4C];
    unsigned int count;
    KindEntry *entries;
} KindTable;

typedef struct KindView {
    unsigned char active;
    char pad_01[0x3];
    KindTable *table;
} KindView;

int func_08AA7A6C(KindView *self, unsigned int index)
{
    if (self->active == 0) {
        return 0;
    }
    if (index >= self->table->count) {
        return 0;
    }
    return self->table->entries[index].high == 2;
}
