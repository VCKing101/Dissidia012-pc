/* Resets a record: base init, clears the head fields and slot table, constructs six sub-records, then sets the defaults. */
typedef struct S { char p0[0xC]; float f0C; float f10; float f14; float f18; char p1[0x0C]; int f28; unsigned char f2C; char p2[3]; int f30; int slot[1]; char sub[6][0x5C]; char p3[0x260 - 0x38 - 6 * 0x5C]; int f260; int f264; char p4[0x294 - 0x268]; int f294; unsigned char f298; unsigned char f299; char p5[0x2C4 - 0x29A]; float f2C4; float f2C8; } S;
extern void func_0892AC2C(S *self);
extern void func_08930328(void *p);
void func_08930BE4(S *self)
{
    int i;
    func_0892AC2C(self);
    self->f28 = 0;
    self->f2C = 0;
    self->f30 = 0;
    for (i = 0; i < 1; i++) {
        self->slot[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        func_08930328(self->sub[i]);
    }
    self->f260 = 0;
    self->f264 = 0;
    self->f294 = 0;
    self->f298 = 1;
    self->f299 = 1;
    self->f2C4 = 0.0f;
    self->f2C8 = -1.0f;
    self->f0C = 0.0f;
    self->f10 = 0.0f;
    self->f14 = 0.0f;
    self->f18 = 0.0f;
}
