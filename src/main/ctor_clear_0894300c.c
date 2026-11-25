/* Constructor: builds the member at +0x28, clears the fields, sets the id byte to 12 and zeroes the 0x34-byte block at +0xB4. */
typedef struct S {
    int a; float b, c;
    char p0[0x18];
    short s24;
    char p1[2];
    char member[0x5C];
    char c84, c85;
    char p2[0xE];
    char c94;
    char p3[7];
    int i9c, ia0, ia4, ia8, iac, ib0;
} S;
extern void func_08970EE8(void *m);
extern void func_08935828(void *p, int v, int n);
S *func_0894300C(S *self) {
    func_08970EE8(self->member);
    self->a = 0;
    self->b = 0.0f;
    self->c = 0.0f;
    self->c94 = 12;
    self->c84 = 0;
    self->c85 = 0;
    self->s24 = 0;
    self->ia4 = 0;
    self->i9c = 0;
    self->ia0 = 0;
    self->ia8 = 0;
    self->iac = 0;
    self->ib0 = 0;
    func_08935828((char *)self + 0xB4, 0, 0x34);
    return self;
}
