/* Menu item tick: counts down the delay, then releases the item handle; returns 1 when idle with the flag set. */
typedef struct { char p[0x20]; unsigned short h20; char q[0x30 - 0x22]; int f30; int f34; int f38; } It;
typedef struct {
    int f0; It **f4; int *f8; char p[0x14 - 0xC]; int f14; int f18;
    char q[0xF4 - 0x1C]; int fF4; unsigned char bF8;
} A;
extern char D_08AF99C0[];
extern void func_0887BA5C(int);
extern void func_0887BA80(int, void *, A *);
extern void func_08AF9880(int);
int func_08AF99CC(int x, A *a) {
    int r = 0;
    It *it;
    int go;
    func_08AF9880(x);
    it = a->f4[a->f14];
    go = (it->h20 & 0x20) != 0;
    go = (unsigned char)go;
    if (go) {
        if (a->fF4 <= 0) {
            if (a->f8[a->f18] != 0) {
                it->f34 = 0;
                it->f30 = 0;
                it->f38 = 0;
                func_0887BA80(a->f8[a->f18], D_08AF99C0, a);
                if (a->f8[a->f18] != 0) {
                    func_0887BA5C(a->f8[a->f18]);
                    a->f8[a->f18] = 0;
                }
            } else if (a->bF8 != 0) {
                r = 1;
            }
        } else {
            a->fF4 = a->fF4 - 1;
        }
    }
    return r;
}
