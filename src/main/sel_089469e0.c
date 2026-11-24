/* Looks up a 16-bit value through a table; -1 when the index is negative or bit 12 is clear. */
int func_089469E0(int a0, int *tbl, int idx) {
    if (idx < 0) { return -1; }
    if (!(idx & 0x1000)) { return -1; }
    int mask = -0x1001;
    int *row = (int *)tbl[0x2C / 4];
    int *e = (int *)row[idx & mask];
    return *(unsigned short *)e;
}
