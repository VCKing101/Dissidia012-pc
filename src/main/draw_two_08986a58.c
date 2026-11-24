// Draws the two halves with 2/3 of the width at +0x20, or the single combined draw when the flag at +0xC0 is set; first runs the pre-pass when +0xB3 is set.
struct S { char pad[0x1C]; int a; int w; char pad2[0x20]; int p; int q; char pad3[0x67]; unsigned char pre; char pad4[0xC]; unsigned char one; };
extern void func_08986D38(struct S *, int);
extern void func_08986CC8(struct S *, int, int, int, int);
extern void func_08986BC4(struct S *, int);
void func_08986A58(struct S *s, int x) {
    int v;
    if (s->pre != 0) {
        func_08986D38(s, x);
    }
    if (s->one != 0) {
        func_08986BC4(s, x);
    } else {
        v = (s->w / 3) * 2;
        func_08986CC8(s, x, s->a, s->p, v);
        func_08986CC8(s, x, s->a, s->q, v);
    }
}
