/* Returns value i of a group from the third value table: -1 when the table is
   invalid, -2 without a group or out of range. */
typedef struct Group0881E898 {
    char pad_00[0x1C];
    short count;
    short start;
} Group0881E898;

typedef struct Table0881E898 {
    unsigned char valid;
    char pad_01[0x73];
    int *values;
} Table0881E898;

int func_0881E898(Table0881E898 *t, Group0881E898 *g, int i)
{
    if (!t->valid) {
        return -1;
    }
    if (g == 0 || i >= g->count) {
        return -2;
    }
    return t->values[g->start + i];
}
