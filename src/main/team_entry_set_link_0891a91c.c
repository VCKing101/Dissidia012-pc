typedef struct EntryLink {
    short id;
    signed char on;
    signed char mode;
    int value;
    int target;
} EntryLink;

typedef struct EntryLinkKeys {
    unsigned short id;
    unsigned char on;
    unsigned char mode;
    unsigned int value;
    unsigned int target;
} EntryLinkKeys;

typedef struct TeamEntry {
    short kind;
    char pad_02[0xE];
    union {
        EntryLink link;
        EntryLinkKeys keys;
    } u;                        /* 0x10 */
    char pad_1C[0x2E8 - 0x1C];
} TeamEntry;

typedef struct Team {
    int field_000;
    TeamEntry entries[2];       /* 0x004 */
} Team;

void func_0891A91C(Team *team, int index, int id, int on, int mode, int value, int target)
{
    TeamEntry *entry = &team->entries[index];
    EntryLink *link = &entry->u.link;
    entry->kind = 2;
    link->id = -1;
    link->on = 1;
    link->mode = 0;
    link->value = 0;
    link->target = -1;
    ((EntryLinkKeys *)link)->id = id;
    ((EntryLinkKeys *)link)->on = on;
    ((EntryLinkKeys *)link)->mode = mode;
    ((EntryLinkKeys *)link)->value = value;
    ((EntryLinkKeys *)link)->target = target;
}
