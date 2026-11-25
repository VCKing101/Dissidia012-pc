/* Unless the query returns 1, packs the owner's fifteen state values into a 0x14-byte record (bytes 0..0xE, halfwords at +0x10 and +0x12) and sends it with func_08A6FC24, size 0x12. */
typedef struct Rec08AB5D58 { unsigned char b[0x10]; short h10; short h12; } Rec08AB5D58;
typedef struct S08AB5D58 {
    char pad[0x4C];
    short s4C, s4E, s50, s52, s54, s56, s58, s5A, s5C, s5E, s60, s62, s64, s66, s68, s6A;
} S08AB5D58;
extern int func_08AB6DAC(void);
extern int func_08AB6D58(S08AB5D58 *self);
extern void func_08A6FC24(int owner, int n, Rec08AB5D58 *rec, int size);
void func_08AB5D58(S08AB5D58 *self)
{
    Rec08AB5D58 rec;
    int owner;
    int w;
    if (func_08AB6DAC() == 1) {
        return;
    }
    owner = func_08AB6D58(self);
    w = *(int *)(owner + 0x210);
    rec.b[0] = self->s4C;
    rec.b[1] = self->s4E;
    rec.b[2] = self->s50;
    rec.b[3] = self->s52;
    rec.b[4] = self->s54;
    rec.b[5] = self->s56;
    rec.b[6] = self->s5A;
    rec.b[8] = self->s5C;
    rec.b[9] = self->s5E;
    rec.b[7] = self->s60;
    rec.h10 = self->s58;
    rec.b[10] = self->s62;
    rec.b[11] = self->s64;
    rec.b[12] = self->s66;
    rec.b[13] = self->s68;
    rec.b[14] = self->s6A;
    rec.h12 = w;
    func_08A6FC24(owner, 0x12, &rec, 0x14);
}
