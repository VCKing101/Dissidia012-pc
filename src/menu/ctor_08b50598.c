/* Constructor: base init, clears two words, two more in a loop, constructs five 0x24-byte entries and zeroes the remaining fields. */
typedef struct E50598 { int w[9]; } E50598;
typedef struct O50598 {
    char p0[0xC]; float f[4]; char p1[0x28 - 0x1C]; int a; int b; int c[2];
    char p2[0x38 - 0x38]; E50598 e[5]; char p3[0xEC - 0xEC + 0]; int z[4];
} O50598;
extern void func_0892AC2C(O50598 *);
extern void func_08B4FE98(E50598 *);
void func_08B50598(O50598 *self)
{
    int i;
    int *p;
    E50598 *e;
    func_0892AC2C(self);
    self->a = 0;
    self->b = 0;
    for (i = 0, p = (int *)self; i < 2; i++, p++) {
        p[0x30 / 4] = 0;
    }
    for (i = 0, e = self->e; i < 5; i++, e++) {
        func_08B4FE98(e);
    }
    self->z[0] = 0; self->z[1] = 0; self->z[2] = 0; self->z[3] = 0;
    self->f[0] = 0.0f; self->f[1] = 0.0f; self->f[2] = 0.0f; self->f[3] = 0.0f;
}
