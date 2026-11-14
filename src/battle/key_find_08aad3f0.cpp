typedef struct KeyEntry {
    int key;
    char pad_04[0x24];
} KeyEntry;

typedef struct KeyList {
    int count;
    KeyEntry *entries;
} KeyList;

typedef struct KeyInfo {
    char pad_00[0x2];
    short key;
} KeyInfo;

typedef struct KeyOwner {
    char pad_00[0x30];
    KeyList list;
} KeyOwner;

extern "C" {

extern KeyInfo *func_08AAC380(KeyOwner *self, int id);

KeyEntry *func_08AAD3F0(KeyOwner *self, int id)
{
    KeyInfo *info = func_08AAC380(self, id);
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
