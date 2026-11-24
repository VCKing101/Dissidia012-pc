// Starts a white flash: sets colour (255,255,255,255) via func_08873DB0, begins a timed effect of func_088D4C5C() frames, then stores the duration at +0x24, clears +0x20, state 4 and the flag byte.
extern int func_088D4C5C(void);
extern void func_08873DB0(float *, int);
extern void func_08873D34(float, int, int, int, int, int);
typedef struct Flash57 {
    char pad0[4];
    int state;
    char pad1[0x18 - 8];
    unsigned char flag;
    char pad2[0x20 - 0x19];
    float f20;
    float f24;
} Flash57;
int func_08B1E0C4(Flash57 *self, unsigned char flag)
{
    float c[4];
    int n = func_088D4C5C();
    c[0] = 255.0f;
    c[1] = 255.0f;
    c[2] = 255.0f;
    c[3] = 255.0f;
    func_08873DB0(c, 2);
    func_08873D34((float)n, 3, 0, 0, 0, 2);
    self->f20 = 0.0f;
    self->f24 = (float)n;
    self->state = 4;
    self->flag = flag;
    return 0;
}
