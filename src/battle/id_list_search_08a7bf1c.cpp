/* Searches of a list of bank object ids (up to the first unused entry):
 * the last object reached before one without flag 0x1000, whether any
 * object carries flag 0x10, and whether any object is active. */
struct MarkedEntry {
    short id;
    unsigned char pad_02[6];
};

struct MarkedList {
    unsigned char pad_00[2];
    short count;
    unsigned char pad_04[4];
    MarkedEntry entries[1];
};

struct MarkedObject {
    unsigned char pad_00[0x15];
    unsigned char state;
    unsigned char pad_16[2];
    unsigned int flags;

    static bool marked(MarkedObject *object) { return (object->flags & 0x10) != 0; }
};

struct MarkedBankRef {
    unsigned char pad_00[4];
    void *bank;
};

struct MarkedOwner {
    unsigned char pad_00[4];
    MarkedBankRef *ref;
};

extern "C" MarkedObject *func_08AB332C(void *bank, int id);

extern "C" MarkedObject *func_08A7BF1C(MarkedOwner *self, MarkedList *list)
{
    if (list == 0) {
        return 0;
    }
    void *bank = self->ref->bank;
    if (bank == 0) {
        return 0;
    }
    MarkedObject *found = 0;
    for (int i = 0; i < list->count; i++) {
        MarkedEntry *entry = &list->entries[i];
        if (entry->id < 0) {
            break;
        }
        MarkedObject *object = func_08AB332C(bank, entry->id);
        if (object == 0) {
            continue;
        }
        found = object;
        if ((object->flags & 0x1000) == 0) {
            break;
        }
    }
    return found;
}

extern "C" int func_08A7C004(MarkedOwner *self, MarkedList *list)
{
    if (list == 0) {
        return 0;
    }
    void *bank = self->ref->bank;
    if (bank == 0) {
        return 0;
    }
    int found = 0;
    for (int i = 0; i < list->count; i++) {
        MarkedEntry *entries = list->entries;
        int id = entries[i].id;
        if (id < 0) {
            break;
        }
        MarkedObject *object = func_08AB332C(bank, id);
        if (object == 0) {
            continue;
        }
        if (!MarkedObject::marked(object)) {
            continue;
        }
        found = 1;
        break;
    }
    return found;
}

extern "C" int func_08A7C0F4(MarkedOwner *self, MarkedList *list)
{
    if (list == 0) {
        return 0;
    }
    void *bank = self->ref->bank;
    if (bank == 0) {
        return 0;
    }
    int found = 0;
    for (int i = 0; i < list->count; i++) {
        MarkedEntry *entries = list->entries;
        int id = entries[i].id;
        if (id < 0) {
            break;
        }
        MarkedObject *object = func_08AB332C(bank, id);
        if (object == 0) {
            continue;
        }
        if ((object->state & 1) == 0) {
            continue;
        }
        found = 1;
        break;
    }
    return found;
}
