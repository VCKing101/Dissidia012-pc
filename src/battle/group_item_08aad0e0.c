typedef struct GroupItem {
    int id;
    int value;
} GroupItem;

typedef struct ItemList {
    unsigned int count;
    GroupItem *items;
} ItemList;

typedef struct ItemGroup {
    char pad_00[0x18];
    ItemList list;
    char pad_20[0x4];
} ItemGroup;

typedef struct GroupSet {
    unsigned char active;
    char pad_001[0x1FB];
    ItemGroup groups[3];
} GroupSet;

GroupItem *func_08AAD0E0(GroupSet *self, unsigned int group, unsigned int index)
{
    ItemGroup *groups;
    ItemList *list;

    if (self->active == 0) {
        return 0;
    }
    if (group >= 3) {
        return 0;
    }
    groups = self->groups;
    list = &groups[group].list;
    if (index >= list->count) {
        return 0;
    }
    return &list->items[index];
}
