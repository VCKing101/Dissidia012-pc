struct KindSpan {
    short start;
    short count;
};

struct KindGroup {
    char pad_00[0x4];
    KindSpan span;
};

struct KindEntry {
    short kind;
    char pad_02[0x2];
};

struct KindList {
    unsigned int size;
    KindEntry *entries;
};

struct KindTable {
    unsigned char active;
    char pad_01[0xFB];
    KindList list;
};

extern "C" {

extern KindGroup *func_08AAD384(KindTable *self, int key);

/* The entry of kind in key's group, or 0. */
KindEntry *func_08AAC4B8(KindTable *self, int key, int kind)
{
    if (self->active == 0) {
        return 0;
    }
    KindGroup *group = func_08AAD384(self, key);
    if (group == 0) {
        return 0;
    }
    KindSpan *span = &group->span;
    KindList *list = &self->list;
    for (int i = 0; i < span->count; i++) {
        unsigned int index = span->start + i;
        if (index >= list->size) {
            return 0;
        }
        KindEntry *entry = &list->entries[index];
        if (entry->kind == kind) {
            return entry;
        }
    }
    return 0;
}

}
