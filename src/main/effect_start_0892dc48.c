/* Sets up a timed effect: stores the duration and start fraction, runs two init steps, then marks it active. */
typedef struct S { char p0[0x28]; int state; char p1[0x10]; int ext; float zero; float dur; } S;
extern void func_0892E044(S *self);
extern void func_0892E11C(S *self, int a, int b, int c, int d, int e);
extern void func_088D4BF4(int n);
void func_0892DC48(S *self, int a, int b, int c, int d, int e, int f, float dur)
{
    self->ext = f;
    self->dur = dur;
    self->zero = 0.0f;
    func_0892E044(self);
    func_0892E11C(self, a, b, c, d, e);
    func_088D4BF4(8);
    self->state = 1;
}
