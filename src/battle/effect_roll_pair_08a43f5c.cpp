struct Pi { int lo, hi; };
struct Parts { unsigned char pad[0x34]; int kind; int p38; float f3c; };
struct Eff { unsigned char pad[0x14]; Parts *parts; };
extern int D_09AF8B00;
extern const char D_08A455C0[];
extern const char D_08A455F8[];
struct BR {
    static int next() { return (D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF; }
};
extern "C" {
extern int func_08A40730(Eff *self);
extern void *func_08935524(void *dst, const void *src, unsigned int n);
void func_08A43F5C(Eff *self)
{
    int level = func_08A40730(self);
    int chance[7] = {100, 90, 80, 60, 40, 20, 0};
    int chance2[7] = {100, 90, 80, 80, 70, 60, 50};
    int lo;
    Pi a[7];
    Pi b[7];
    func_08935524(a, D_08A455C0, 0x38);
    func_08935524(b, D_08A455F8, 0x38);
    if (BR::next() % 101 <= chance[level]) {
        self->parts->kind = 0;
        self->parts->f3c = (float)(BR::next() % ((lo = a[level].lo), a[level].hi - lo + 1) + lo);
    } else {
        self->parts->kind = BR::next() % 101 <= chance2[level] ? 1 : 2;
        self->parts->f3c = (float)(BR::next() % ((lo = b[level].lo), b[level].hi - lo + 1) + lo);
    }
}
}
