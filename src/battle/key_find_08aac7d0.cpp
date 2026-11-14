typedef struct KeyEntry {
    int key;
    char pad_04[0x14];
} KeyEntry;

typedef struct KeyList {
    int count;
    KeyEntry *entries;
} KeyList;

typedef struct KeyInfo {
    char pad_00[0x2];
    signed char key;
} KeyInfo;

typedef struct KeyOwner {
    unsigned char active;
    char pad_01[0xBF];
    KeyList list;
} KeyOwner;

extern "C" {

extern KeyInfo *func_08AAC3EC(KeyOwner *self, int a, int b);

KeyEntry *func_08AAC7D0(KeyOwner *self, int a, int b)
{
    if (!self->active) {
        return 0;
    }
    KeyInfo *info = func_08AAC3EC(self, a, b);
    if (info == 0) {
        return 0;
    }
    KeyList *list = &self->list;
    for (int i = 0; i < list->count; i++) {
        KeyEntry *entry = &list->entries[i];
        if (entry->key == info->key) {
            return entry;
        }
    }
    return 0;
}

}
