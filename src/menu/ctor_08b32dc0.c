/* Constructor: fills a block of float parameters (scales, durations, defaults), constructs a sub-object and returns self. */
typedef struct O32DC0 {
    int w0; int w4; int w8;
    float f0C, f10, f14, f18, f1C, f20, f24, f28, f2C, f30;
    char pad0[0x44 - 0x34];
    float f44, f48;
    char pad1[0x58 - 0x4C];
    float f58, f5C;
    char pad2[0xAC - 0x60];
    float fAC, fB0;
    char sub[1];
} O32DC0;
extern float D_08B343C4;
extern float D_08B343C8;
extern void func_08864C1C(void *);
O32DC0 *func_08B32DC0(O32DC0 *self)
{
    float zero;
    zero = 0.0f;
    self->f0C = 10.0f;
    self->f10 = 15.0f;
    self->f14 = 20.0f;
    self->f18 = 10.0f;
    self->f1C = zero;
    self->f20 = 5.0f;
    self->f24 = zero;
    self->f28 = 1.0f;
    self->f2C = zero;
    func_08864C1C((char *)self + 0xB8);
    self->w4 = 0;
    self->w8 = 0;
    self->f30 = 30.0f;
    self->f44 = 45.0f;
    self->f48 = zero;
    self->f5C = D_08B343C4;
    self->f58 = D_08B343C8;
    self->fB0 = -1.0f;
    self->fAC = zero;
    return self;
}
