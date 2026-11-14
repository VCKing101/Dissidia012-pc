typedef struct ModeEntry {
    char pad_00[0xC];
    unsigned char locked;
    char pad_0D[0x13];
} ModeEntry;

typedef struct ModeTable {
    char pad_00[0x10];
    ModeEntry entries[1];
} ModeTable;

typedef struct ModeRef {
    ModeTable *table;
    signed char index;

    static bool valid(ModeTable *table) { return table != 0; }
} ModeRef;

extern "C" {

int func_08ACF234(ModeRef *self)
{
    if (!ModeRef::valid(self->table)) {
        return 0;
    }
    ModeEntry *entries = self->table->entries;
    if (entries[self->index].locked != 0) {
        return 0;
    }
    int modes[3] = {0, 1, 2};
    return modes[self->index];
}

}
