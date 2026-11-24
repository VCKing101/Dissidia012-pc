// Runs func_08B48294(self, a1), func_08B48354(self, a1, a2), func_08B471BC(self+0xB0, +0x30, +0x44), closes +0x40 and sets the state at +0x28 to 1.
extern void func_08B48294(void *, void *);
extern void func_08B48354(void *, void *, void *);
extern void func_08B471BC(void *, int, int);
extern void func_088D9CFC(void *);
typedef struct { char pad0[0x28]; int state; int w2c; int w30; char pad1[0x40 - 0x34]; void *p40; int w44; char pad2[0xB0 - 0x48]; char sub[1]; } Obj08B47B20;
void func_08B47B20(Obj08B47B20 *self0, void *a0, void *b0)
{
    void *b = b0;
    Obj08B47B20 *self = self0;
    void *a = a0;
    int one;
    func_08B48294(self, a);
    func_08B48354(self, a, b);
    func_08B471BC(self->sub, self->w30, self->w44);
    func_088D9CFC(self->p40);
    one = 1;
    self->state = one;
}
