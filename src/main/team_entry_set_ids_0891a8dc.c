typedef struct EntryIds {
    union {
        short ids[3];
        unsigned short keys[3];
    } u;
} EntryIds;

typedef struct TeamEntry {
    short kind;
    char pad_02[0xE];
    EntryIds ids;               /* 0x10 */
    char pad_16[0x2E8 - 0x16];
} TeamEntry;

typedef struct Team {
    int field_000;
    TeamEntry entries[2];       /* 0x004 */
} Team;

void func_0891A8DC(Team *team, int index, int a, int b, int c)
{
    TeamEntry *entry = &team->entries[index];
    EntryIds *ids = &entry->ids;
    entry->kind = 1;
    ids->u.ids[0] = -1;
    ids->u.ids[1] = -1;
    ids->u.ids[2] = -1;
    ids->u.keys[0] = a;
    ids->u.keys[1] = b;
    ids->u.keys[2] = c;
}
