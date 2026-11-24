/* Returns the second argument when the global's flag bit 0 is set (after notifying it), otherwise the looked-up record. */
extern int D_089D8140;
extern int D_089D712C;
extern int func_088CB164(int base, int id);
extern void func_08900CBC(int obj, int a, int b);
int func_08AAA718(int unused, int b, int id) {
    if ((*(unsigned *)(D_089D8140 + 0x1C) & 1) != 0) {
        func_08900CBC(D_089D8140, b, id);
        return b;
    } else {
        return func_088CB164(D_089D712C + 0xF58, id);
    }
}
