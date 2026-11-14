typedef struct Entry48 {
    char data[0x48];
} Entry48;

typedef struct EntrySet5C78 {
    unsigned char active;
    char pad_01[0x3];
    Entry48 entries[3];
    char pad_DC[0x18];
    int field_F4;
    float field_F8;
    float field_FC;
} EntrySet5C78;

extern int func_089F5604(Entry48 *entry);

int func_089F5C78(EntrySet5C78 *self, unsigned int index)
{
    Entry48 *entries;
    if (!self->active) {
        return 0;
    }
    if (index >= 3) {
        return 0;
    }
    if (self->field_F4 == 0 && self->field_FC < self->field_F8) {
        return 1;
    }
    entries = self->entries;
    return func_089F5604(&entries[index]);
}
