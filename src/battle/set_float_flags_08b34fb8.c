// Stores D_08B3ABE0 at +0x1290, sets bit 0 of byte 4 of the flag record at +0x5B0 and bit 13 of the word at +0x5C0.
extern float D_08B3ABE0;
typedef struct { unsigned char b[8]; } Flags08B34FB8;
typedef struct { char pad0[0x5B0]; Flags08B34FB8 fl; char pad1[0x5C0 - 0x5B8]; unsigned w; char pad2[0x1290 - 0x5C4]; float f; } Obj08B34FB8;
void func_08B34FB8(Obj08B34FB8 *self)
{
    Flags08B34FB8 *p;
    self->f = D_08B3ABE0;
    p = &self->fl;
    p->b[4] |= 1;
    self->w |= 0x2000;
}
