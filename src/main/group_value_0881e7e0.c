/* Returns value i of a group from the first value table: -1 when the table is
   invalid, -2 without a group or out of range. */
typedef struct Group0881E7E0 {
    char pad_00[0x14];
    short count;
    short start;
} Group0881E7E0;

typedef struct Table0881E7E0 {
    unsigned char valid;
    char pad_01[0x5B];
    int *values;
} Table0881E7E0;

int func_0881E7E0(Table0881E7E0 *t, Group0881E7E0 *g, int i)
{
    if (!t->valid) {
        return -1;
    }
    if (g == 0 || i >= g->count) {
        return -2;
    }
    return t->values[g->start + i];
}
