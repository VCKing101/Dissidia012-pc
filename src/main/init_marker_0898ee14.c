/* Initialises a marker record: zero timer, active flag, builds the member at +0x40, copies three shorts and two values. */
typedef struct S {
    char p0[0x20]; float t; unsigned char on;
    char p1[7]; int i2c;
    char p2[0x10]; char member[0x10];
    short x, y, z; char p3[2]; int a58; int a5c;
} S;
extern void func_089A3868(void *m);
void func_0898EE14(S *self, int unused, short *v, int a, int b) {
    unsigned char one = 1;
    self->t = 0.0f;
    self->on = one;
    self->i2c = 0;
    func_089A3868(self->member);
    self->x = v[0];
    self->y = v[1];
    self->z = v[2];
    self->a58 = a;
    self->a5c = b;
}
