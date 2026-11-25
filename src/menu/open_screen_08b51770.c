/* Marks the screen active, stores a value and runs a fixed sequence of setup calls before the follow-up call. */
typedef struct S51770 { char pad0[8]; int v; char pad1[5]; unsigned char on; } S51770;
extern int D_08B41CC0;
extern int D_089D6AE8;
extern void func_08B3C944(int, int);
extern void func_088A28E8(int, int);
extern void func_088965F4(int, int);
extern void func_088A3F2C(int);
extern void func_08864D98(float, float);
extern void func_08B51D34(S51770 *);
void func_08B51770(S51770 *self, int v)
{
    self->on = 1;
    self->v = v;
    func_08B3C944(D_08B41CC0, 2);
    func_088A28E8(0, 1);
    func_088965F4(D_089D6AE8, 1);
    func_088A3F2C(1);
    func_08864D98(0.0f, 0.0f);
    func_08B51D34(self);
}
