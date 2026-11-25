// Return false when the requested media (1 = disc, 2 = memory stick) is not usable, else true.
extern "C" char D_089CCB90[];
extern "C" unsigned func_089C5BC0(void);
extern "C" int func_089C5E20(char *name, int code, int a, int b, int *out, int n);
struct E {
    static unsigned eq(int a, int b) { return a == b; }
};
extern "C" bool func_088EE5E0(unsigned kind)
{
    unsigned r;
    int v;
    kind = kind & 0xF;
    if (kind != 0) {
        if (kind == 1) {
            r = func_089C5BC0();
            if ((r & 0x20) == 0) {
                return false;
            }
        } else if (kind == 2) {
            r = func_089C5E20(D_089CCB90, 0x2425823, 0, 0, &v, 4);
            if ((int)r >= 0) { r = E::eq(v, 1); } else { r = r; }
            if (r == 0) {
                return false;
            }
        }
    }
    return true;
}
