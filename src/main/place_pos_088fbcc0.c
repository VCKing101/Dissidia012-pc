/* Looks up a position for the table entry (D_089D7F58) and passes it, truncated to ints, to the sprite call. */
typedef struct Lay { char p[0xA68]; void *h; void *g; char q[0x634]; int a; char r[8]; int b; } Lay;
typedef struct S { char p[0x30]; Lay *lay; } S;
extern unsigned short D_089D7F58[];
extern void func_088DA0A8(void *h, int idx, float *out, float *out2);
extern void func_088D9C60(void *g, int x, int y);
void func_088FBCC0(S *self) {
    float v[16]; int w;
    func_088DA0A8(self->lay->h, D_089D7F58[self->lay->a - self->lay->b], v, (float *)&w);
    func_088D9C60(self->lay->g, (int)v[12], (int)v[13]);
}
