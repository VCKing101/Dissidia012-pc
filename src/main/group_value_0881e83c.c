/* Returns value i of a group from the second value table: -1 when the table is
   invalid, -2 without a group or out of range. */
typedef struct Group0881E83C {
    char pad_00[0x18];
    short count;
    short start;
} Group0881E83C;

typedef struct Table0881E83C {
    unsigned char valid;
    char pad_01[0x67];
    int *values;
} Table0881E83C;

int func_0881E83C(Table0881E83C *t, Group0881E83C *g, int i)
{
    if (!t->valid) {
        return -1;
    }
    if (g == 0 || i >= g->count) {
        return -2;
    }
    return t->values[g->start + i];
}
