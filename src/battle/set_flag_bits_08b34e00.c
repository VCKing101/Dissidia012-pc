// ORs the 64-bit mask D_08B3AD20 into the doubleword at +0x388 and sets bit 7 of byte 6 of the flag record at +0x5B0.
extern unsigned long long D_08B3AD20;
typedef struct { unsigned char b[8]; } Flags08B34E00;
typedef struct { char pad0[0x388]; unsigned long long mask; char pad1[0x5B0 - 0x390]; Flags08B34E00 fl; } Obj08B34E00;
void func_08B34E00(Obj08B34E00 *self)
{
    Flags08B34E00 *f;
    self->mask |= D_08B3AD20;
    f = &self->fl;
    f->b[6] |= 0x80;
}
