struct Pr { float a, b; };
struct Parts { unsigned char pad[0x168]; int outcome; float f16c; float f170; };
struct Eff { unsigned char pad[0x14]; Parts *parts; };
extern int D_09AF8B00;
extern const char D_08A43AC0[];
extern const char D_08A43AF8[];
struct BR {
    static int next() { return (D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF; }
};
extern "C" {
extern int func_08A40730(Eff *self);
extern void *func_08935524(void *dst, const void *src, unsigned int n);
void func_08A42194(Eff *self)
{
    int level = func_08A40730(self);
    int chance[7] = {100, 90, 80, 60, 40, 20, 0};
    Pr a[7];
    Pr b[7];
    float lo, hi;
    func_08935524(a, D_08A43AC0, 0x38);
    func_08935524(b, D_08A43AF8, 0x38);
    if (BR::next() % 101 <= chance[level]) {
        float x0 = a[level].a;
        float x1 = a[level].b;
        self->parts->f16c = (float)BR::next() * (1.0f / 32767.0f) * (x1 - x0) + x0;
    } else {
        self->parts->outcome = 1;
    }
    lo = b[level].a;
    hi = b[level].b;
    self->parts->f170 = (float)BR::next() * (1.0f / 32767.0f) * (hi - lo) + lo;
}
}
