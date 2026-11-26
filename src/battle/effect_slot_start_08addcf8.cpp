/* Picks the sound/effect id for a battle effect slot and starts it, or clears the slot's fields. */
struct Q { char p[0x1C]; short v; };
struct E { char p0[0x12C]; short a; char p1[0x40]; short b; int c, d, e; };
extern "C" {
extern Q *func_088BBD8C(int a);
extern int func_088CAF10(int a);
extern void func_08ADDFE4(int a, float f, E *p, int c, int d, unsigned char e, unsigned char g, unsigned char h, int i, int j, int k, int l);
extern void func_08ADDCF8(int a0, float f, E *p, int a2, unsigned mode, int t0, int idx, unsigned char b0, unsigned char b1, unsigned char b2, int s5, int s4, int s3) {
    int id = -1;
    if (idx >= 0) {
        Q *e = func_088BBD8C(idx);
        if (e) id = e->v;
    }
    if (id < 0) {
        switch (mode) {
        case 0: id = 0x41; break;
        case 2: id = 0x10; break;
        case 3: id = 0x12; break;
        case 4: id = 0x14; break;
        case 1: id = 0x11; break;
        case 5: id = 0x42; break;
        default: break;
        }
    }
    if (id >= 0) {
        int r = func_088CAF10(id); int t = t0;
        func_08ADDFE4(a0, f, p, a2, r, b0, b1, b2, s5, t, s4, s3);
    } else {
        p->a = 0; p->b = 0; p->c = 0; p->d = 0; p->e = 0;
    }
}
}
