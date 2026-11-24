/* Returns -1 for out-of-range ids (1..0x22), 0 for 0x17, otherwise a signed byte from the looked-up record. */
extern int D_089D712C;
extern int func_088CB164(int base, int id);
int func_08A816E0(int a, int id) {
    char *q;
    a = id;
    if (id <= 0 || a >= 0x23) {
        return -1;
    }
    if (a == 0x17) {
        return 0;
    }
    q = (char *)(func_088CB164(D_089D712C + 0xF58, a) + 0x10);
    return q[0x763];
}
