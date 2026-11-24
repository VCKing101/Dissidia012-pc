// Starts a timed effect: takes func_088D4C5C()'s value as the duration, passes it to func_088A18B0 and func_08873D34/08873DEC, stores it as a float at +0x10 and clears +0xC/+0x14.
extern int func_088D4C5C(void);
extern void func_088A18B0(int, int, int);
extern void func_08873D34(float, int, int, int, int, int);
extern void func_08873DEC(int, int);
extern int D_089D6C88;
typedef struct Timer57 {
    char pad0[0xC];
    float f0C;
    float f10;
    unsigned char f14;
} Timer57;
void func_08B2CE28(Timer57 *self)
{
    int n = func_088D4C5C();
    func_088A18B0(D_089D6C88, 0, n);
    func_08873D34((float)n, 3, 0, 0, 0, 2);
    func_08873DEC(3, 2);
    self->f0C = 0.0f;
    self->f10 = (float)n;
    self->f14 = 0;
}
