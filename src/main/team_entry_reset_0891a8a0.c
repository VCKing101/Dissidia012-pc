typedef struct EntryCell {
    union {
        signed char id;
        unsigned char key;
    } u;
    signed char x;
    signed char y;
    signed char flags;
} EntryCell;

typedef struct TeamEntry {
    short count;
    char pad_02[0xE];
    EntryCell cell;             /* 0x10 */
    char pad_14[0x2E8 - 0x14];
} TeamEntry;

typedef struct Team {
    int field_000;
    TeamEntry entries[2];       /* 0x004 */
} Team;

void func_0891A8A0(Team *team, int index, int id)
{
    TeamEntry *entry = &team->entries[index];
    EntryCell *cell = &entry->cell;
    entry->count = 0;
    cell->u.id = -1;
    cell->x = 0;
    cell->y = -1;
    cell->flags = 1;
    cell->u.key = id;
}
