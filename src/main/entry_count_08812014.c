typedef struct Agent4Entry20 {
    int field_00;
    unsigned char kind;
    unsigned char unknown_05[0x1B];
} Agent4Entry20;

typedef struct Agent4EntryTable {
    int field_00;
    int field_04;
    int count;
    int field_0C;
    Agent4Entry20 entries[1];
} Agent4EntryTable;

int func_08812014(Agent4EntryTable *self, unsigned int kind) {
    int found = 0;
    int i;
    Agent4Entry20 *entry;
    for (i = 0; i < self->count; i++) {
        entry = &self->entries[i];
        if (entry->kind == kind) {
            found++;
        }
    }
    return found;
}
