struct KindSpan {
    short start;
    short count;
};

struct KindGroup {
    char pad_00[0xC];
    KindSpan span;
};

struct KindEntry {
    signed char kind;
    char pad_01[0x7];
};

struct KindList {
    unsigned int size;
    KindEntry *entries;
};

struct KindTable {
    unsigned char active;
    char pad_01[0x23];
    KindList list;
};

extern "C" {

extern KindGroup *func_08AAC380(KindTable *self, int key);

/* The entry of kind in key's group, or 0. */
KindEntry *func_08AAC3EC(KindTable *self, int key, int kind)
{
    if (self->active == 0) {
        return 0;
    }
    KindGroup *group = func_08AAC380(self, key);
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
