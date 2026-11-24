// Copies four 8-byte rows from the source into the destination (row 0x10 copied twice, as retail does); returns 1.
extern void func_08935524(void *, void *, int);
int func_08956164(int a, int b, char *src, char *dst) {
    char *s = src;
    char *d = dst;
    char *p = d + 8;
    char *q;
    char *r;
    func_08935524(p, s + 0x10, 8);
    func_08935524(d, p, 8);
    q = d + 0x10;
    r = s + 0x18;
    func_08935524(q, r, 8);
    func_08935524(q, r, 8);
    func_08935524(d + 0x18, s + 0x20, 8);
    return 1;
}
