/* Returns value i of a group from the fourth value table: -1 when the table is
   invalid, -2 without a group or out of range. */
typedef struct Group0881E8F4 {
    char pad_00[0x20];
    short count;
    short start;
} Group0881E8F4;

typedef struct Table0881E8F4 {
    unsigned char valid;
    char pad_01[0x7F];
    int *values;
} Table0881E8F4;

int func_0881E8F4(Table0881E8F4 *t, Group0881E8F4 *g, int i)
{
    if (!t->valid) {
        return -1;
    }
    if (g == 0 || i >= g->count) {
        return -2;
    }
    return t->values[g->start + i];
}
