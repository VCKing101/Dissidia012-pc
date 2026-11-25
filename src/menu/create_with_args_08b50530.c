/* Calls func_0892AC5C(self, ...), then stores func_088D0700(D_089D7830, c, d, e) at self+0x2C. */
struct Self { char pad[0x2C]; int h; };
extern void *D_089D7830;
extern void func_0892AC5C(struct Self *self);
extern int func_088D0700(void *o, unsigned char a, int b, int c);
void func_08B50530(struct Self *self, int a1, int a2, int a3, int c, int d, int e)
{
    int e2 = e;
    int d2 = d;
    struct Self *s2 = self;
    int c2 = c;
    func_0892AC5C(s2);
    int b = c2 & 0xFF;
    s2->h = func_088D0700(D_089D7830, b, d2, e2);
}
