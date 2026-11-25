/* start the object's timer, clear its flag bit and refresh it. */
extern void func_08933C28(int a);
extern void func_08B49CF4(int a);
extern int func_088D4C5C(void);
extern void func_08873D34(int a, void *b, void *c, int d, int e, float f);
extern void func_08873DB0(float *v, int a);
extern void func_08914BC8(void *s);
extern void func_08914B64(void *s, int a);
extern int D_089D85E8;
typedef struct Flags { char pad[0x380]; int f; } Flags;
typedef struct Obj { char pad0[0x570]; int f570; char pad1[0x6D0 - 0x574]; Flags *fl; char pad2[0x7E9 - 0x6D4]; char f7E9; } Obj;
void func_08914838(Obj *self)
{
    float v[4];
    float z;
    func_08933C28(D_089D85E8);
    self->f7E9 = 1;
    func_08B49CF4(self->f570);
    func_08873D34(2, 0, 0, 0, 2, (float)func_088D4C5C());
    z = 0.0f;
    v[0] = z; v[1] = z; v[2] = z; v[3] = z;
    func_08873DB0(v, 2);
    func_08914BC8(self);
    self->fl->f &= ~2;
    func_08914B64(self, 2);
}
