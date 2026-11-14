typedef struct GroupEntry {
    char data[0xC];
} GroupEntry;

typedef struct GroupList {
    unsigned int count;
    GroupEntry *entries;
} GroupList;

typedef struct EntryGroup {
    char pad_00[0xC];
    GroupList list;
    char pad_14[0x10];
} EntryGroup;

typedef struct GroupOwner {
    unsigned char active;
    char pad_001[0x1FB];
    EntryGroup groups[3];
} GroupOwner;

extern "C" {

GroupEntry *func_08AAD070(GroupOwner *self, unsigned int which, unsigned int index)
{
    if (!self->active) {
        return 0;
    }
    if (which >= 3) {
        return 0;
    }
    EntryGroup *groups = self->groups;
    GroupList *list = &groups[which].list;
    if (index >= list->count) {
        return 0;
    }
    return &list->entries[index];
}

}
