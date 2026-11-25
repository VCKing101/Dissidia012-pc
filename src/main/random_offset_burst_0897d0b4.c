typedef struct Src { char pad[0x58]; float rx; float ry; char pad2[0x2C]; unsigned char on; } Src;
typedef struct Out { int h; float x; float y; } Out;
extern void func_08986F0C(int a, void *b, int c, int d, int e);
extern int D_089DA120;
void func_0897D0B4(int a, int b, Src *p, Out *out) {
    func_08986F0C(out->h, p, 0, 1, 2);
    if (p->on != 0) {
        float two = 2.0f;
        out->x = ((float)((D_089DA120 = D_089DA120 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF) / 32767.0f) * two * p->rx - p->rx;
        out->y = ((float)((D_089DA120 = D_089DA120 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF) / 32767.0f) * two * p->ry - p->ry;
    }
}
