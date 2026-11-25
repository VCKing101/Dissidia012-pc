struct Pi { float a, b; };
struct Parts { unsigned char pad[0x5C]; unsigned char kind; unsigned char p[3]; float f60; };
struct Eff { unsigned char pad[0x14]; Parts *parts; };
extern int D_09AF8B00;
extern const char D_08A39900[];
extern const char D_08A39938[];
struct BR {
    static int next() { return (D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF; }
};
extern "C" {
extern int func_08A40730(Eff *self);
extern void *func_08935524(void *dst, const void *src, unsigned int n);
void func_08A38530(Eff *self)
{
    int level = func_08A40730(self);
    int chance[7] = {100, 90, 80, 60, 40, 20, 0};
    Pi a[7];
    Pi b[7];
    func_08935524(a, D_08A39900, 0x38);
    func_08935524(b, D_08A39938, 0x38);
    if (BR::next() % 101 <= chance[level]) {
        int lo = (int)a[level].a;
        int hi = (int)a[level].b;
        self->parts->f60 = (float)(BR::next() % (hi - lo + 1) + lo);
        self->parts->kind = 0;
    } else {
        int lo = (int)b[level].a;
        int hi = (int)b[level].b;
        self->parts->f60 = (float)(BR::next() % (hi - lo + 1) + lo);
        self->parts->kind = BR::next() % 101 < 50 ? 1 : 2;
    }
}
}
