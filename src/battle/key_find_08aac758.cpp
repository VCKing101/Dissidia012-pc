typedef struct KeyEntry {
    int key;
    char pad_04[0x10];
} KeyEntry;

typedef struct KeyList {
    int count;
    KeyEntry *entries;
} KeyList;

typedef struct KeyOwner {
    unsigned char active;
    char pad_01[0xB3];
    KeyList list;
} KeyOwner;

extern "C" {

KeyEntry *func_08AAC758(KeyOwner *self, int key)
{
    if (!self->active) {
        return 0;
    }
    if (key < 0) {
        return 0;
    }
    KeyList *list = &self->list;
    for (int i = 0; i < list->count; i++) {
        KeyEntry *entry = &list->entries[i];
        if (entry->key == key) {
            return entry;
        }
    }
    return 0;
}

}
