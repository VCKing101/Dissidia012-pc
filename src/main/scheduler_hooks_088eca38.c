/* set up the object's scheduler hooks and clear its state. */
extern int func_088D4C5C(void);
extern void func_08873D34(int a, void *b, void *c, int d, int e, float f);
extern void func_08873DB0(float *v, int a);
extern int func_088DAA68(int a);
extern void func_088EC634(void *p);
extern void func_088EC34C(void *p);
typedef struct Arg { char pad[0xC]; int v; } Arg;
typedef struct Obj { char pad0[4]; char a[0x14]; char b[0xC]; int f24; char pad1[4]; int f2C; char f30; } Obj;
void func_088ECA38(Obj *self, Arg *arg)
{
    float v[4];
    float z;
    float f = (float)func_088D4C5C();
    int h = arg->v;
    func_08873D34(2, (void *)0x088ECD98, self, 0, 2, f);
    z = 0.0f;
    v[0] = z; v[1] = z; v[2] = z; v[3] = z;
    func_08873DB0(v, 2);
    self->f24 = func_088DAA68(h);
    func_088EC634(self->a);
    func_088EC34C(self->b);
    self->f30 = 0;
    self->f2C = 0;
}
