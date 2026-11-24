/* Returns a pointer into the per-kind table: base + 12 * (signed byte of the record). */
extern char *D_08AB2344[];
char *func_08AB0F80(char *rec, int kind, int slot) {
    return D_08AB2344[kind * 2] + *(signed char *)(rec + kind * 0x40 + slot + 0x11) * 12;
}
