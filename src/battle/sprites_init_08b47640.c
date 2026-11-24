// Creates three sprites from one resource, stores their ids, and places the second at the given position.
struct Pos { float x, y; };
struct Self { char pad[8]; int h[3]; int h14_dummy; };
struct S2 { int pad0[2]; int h[3]; int hc_dummy; };
extern void func_08B47710(void *);
extern int func_088DAA68(int);
extern void func_088D3CF0(int, void *, void *);
extern void func_088D9C60(int, int, int);

void func_08B47640(int *self, int res, int v14, int v32, struct Pos *p)
{
    int i;
    int *hp;
    short *sp;
    func_08B47710(self);
    for (i = 0, hp = self, sp = (short *)self; i < 3; i++, hp++, sp += 4) {
        hp[2] = func_088DAA68(res);
        sp[0x32 / 2] = v32;
    }
    self[0x14 / 4] = v14;
    func_088D3CF0(0, self + 2, (char *)self + 0x2C);
    func_088D9C60(self[3], (int)p->x, (int)p->y);
}
