struct Pi { int lo, hi; };
struct P2 { unsigned char pad[0x60]; int on[5]; float f74; int idx78; };
struct Eff { unsigned char pad[0x14]; P2 *parts; };
extern int D_09AF8B00;
extern const char D_08A34D90[];
struct BR {
    static int next() { return (D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF; }
};
extern "C" {
extern int func_08A40730(Eff *self);
extern void *func_08935524(void *dst, const void *src, unsigned int n);
void func_08A33AB4(Eff *self)
{
    int level = func_08A40730(self);
    int chance[7] = {100, 90, 80, 60, 40, 20, 0};
    int lo;
    Pi a[7];
    func_08935524(a, D_08A34D90, 0x38);
    self->parts->f74 = (float)(BR::next() % (((lo = a[level].lo), a[level].hi - lo + 1)) + lo);
    self->parts->idx78 = 0;
    if (BR::next() % 101 <= chance[level]) {
        int i = 0;
        int off = 0;
        do {
            i++;
            *(int *)((char *)self->parts + off + 0x60) = 1;
            off += 4;
        } while (i < 3);
    } else {
        self->parts->on[BR::next() % 5] = 1;
        self->parts->on[BR::next() % 5] = 1;
    }
}
}
