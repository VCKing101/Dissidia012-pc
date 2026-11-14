/* Appends an 8-byte entry to list A of group `index` (at most 5). */
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

int func_08907400(GroupTable *t, int index, GroupEntry *entry)
{
    Group *g = &t->groups[index];

    if (g->a_count < 5) {
        g->a[g->a_count] = *entry;
        g->a_count++;
        return 1;
    }
    return 0;
}
