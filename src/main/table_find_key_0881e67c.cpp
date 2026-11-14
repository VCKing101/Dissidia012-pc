extern "C" {
struct Entry {
    int key;
    char pad4[0x1C];
};
struct Group {
    char pad0[0x60];
    int count;
    int start;
};
struct Table {
    unsigned char loaded;
    char pad1[0x43];
    Entry *entries;
};
Entry *func_0881E67C(Table *table, Group *group, int key)
{
    if (table->loaded == 0) {
        return 0;
    }
    if (group == 0) {
        return 0;
    }
    int count = group->count;
    for (int i = 0; i < count; i++) {
        int index = group->start + i;
        Entry *entry = &table->entries[index];
        if (entry->key == key) {
            return entry;
        }
    }
    return 0;
}
}
