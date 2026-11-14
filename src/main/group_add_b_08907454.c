/* Appends an 8-byte entry to list B of group `index` (at most 5). */
typedef struct GroupEntry {
    int first;
    int second;
} GroupEntry;

typedef struct Group {
    char pad_00[8];
    GroupEntry a[5];
    GroupEntry b[5];
    int a_count;
    int b_count;
    char pad_60[0x80 - 0x60];
} Group;

typedef struct GroupTable {
    int field_00;
    Group groups[4];
} GroupTable;

int func_08907454(GroupTable *t, int index, GroupEntry *entry)
{
    Group *g = &t->groups[index];

    if (g->b_count < 5) {
        g->b[g->b_count] = *entry;
        g->b_count++;
        return 1;
    }
    return 0;
}
