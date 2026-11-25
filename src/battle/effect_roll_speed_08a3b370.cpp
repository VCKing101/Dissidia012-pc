struct Pi { int a, b; };
struct Pf { float a, b; };
struct Parts { unsigned char pad[0x14]; float f14; float f18; };
struct Eff { unsigned char pad[0x14]; Parts *parts; };
extern int D_09AF8B00;
extern const char D_08A3C900[];
extern const char D_08A3C938[];
extern const char D_08A3C970[];
struct BR {
    static int next() { return (D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF; }
};
extern "C" {
extern int func_08A40730(Eff *self);
extern void *func_08935524(void *dst, const void *src, unsigned int n);
void func_08A3B370(Eff *self)
{
    int level = func_08A40730(self);
    int chance[7] = {100, 90, 80, 60, 40, 20, 0};
    Pi a[7];
    Pi b[7];
    Pf c[7];
    func_08935524(a, D_08A3C900, 0x38);
    func_08935524(b, D_08A3C938, 0x38);
    func_08935524(c, D_08A3C970, 0x38);
    if (BR::next() % 101 <= chance[level]) {
        int lo = a[level].b;
        int hi = a[level].a;
        self->parts->f14 = 240.0f / (float)(BR::next() % (hi - lo + 1) + lo);
        self->parts->f18 = -1.0f;
    } else {
        int lo = b[level].b;
        int hi = b[level].a;
        self->parts->f14 = 240.0f / (float)(BR::next() % (hi - lo + 1) + lo);
        float flo;
        self->parts->f18 = (float)BR::next() * (1.0f / 32767.0f) * (c[level].a - (flo = c[level].b)) + flo;
    }
}
}
