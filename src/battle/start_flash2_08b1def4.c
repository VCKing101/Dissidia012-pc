// Starts a white flash like 08B1E0C4 and arms the timer: stores the argument at +0x1C, zero at +0x20 and the duration as a float at +0x24, notifies func_088A18B0 and moves to state 2.
extern int func_088D4C5C(void);
extern void func_08873DB0(float *, int);
extern void func_08873D34(float, int, int, int, int, int);
extern void func_088A18B0(int, int, int);
extern int D_089D6C88;
typedef struct Flash57 {
    char pad0[4];
    int state;
    char pad1[0x1C - 8];
    int f1C;
    float f20;
    float f24;
} Flash57;
int func_08B1DEF4(Flash57 *self, int arg)
{
    float c[4];
    int two;
    int n = func_088D4C5C();
    c[0] = 255.0f;
    c[1] = 255.0f;
    c[2] = 255.0f;
    two = 2;
    c[3] = 255.0f;
    func_08873DB0(c, 2);
    func_08873D34((float)n, 3, 0, 0, 0, 2);
    self->f1C = arg;
    self->f20 = 0.0f;
    self->f24 = (float)n;
    func_088A18B0(D_089D6C88, 0, n);
    self->state = two;
    return 0;
}
