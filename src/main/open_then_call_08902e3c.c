/* Zeroes an 8-byte local, copies 8 bytes of name into it, and when func_089C5950(local) is non-negative calls func_08903370(a, c, d); returns whether it succeeded. */
extern void func_08935828(void *p, int v, int n);
extern void func_08935BB4(void *dst, void *src, int n);
extern int func_089C5950(void *p);
extern void func_08903370(int a, int c, int d);
int func_08902E3C(int a, void *name, int c, int d)
{
    char buf[8];
    int r;
    func_08935828(buf, 0, 8);
    func_08935BB4(buf, name, 8);
    r = func_089C5950(buf);
    if (r < 0) {
        return 0;
    } else {
        func_08903370(a, c, d);
        return 1;
    }
}
