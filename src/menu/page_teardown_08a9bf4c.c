/* Page teardown on key flags 0x40 / 0x80: releases the animation objects and resets the state. */
typedef struct Bk { char p0[0x914]; int s914; char p1[0x9B0 - 0x918]; int h9B0; int p9B4; int h9B8; int p9BC; int h9C0; char p2[0x8044 - 0x9C4]; int h8044; char p3[0x81B4 - 0x8048]; unsigned int f81B4; } Bk;
typedef struct S { char p[0x154]; Bk *b; } S;
extern void func_08B2BC40(int a);
extern void func_08AAB8B4(int a, int b);
extern void func_08A9AA84(S *s);
void func_08A9BF4C(S *self)
{
    unsigned int f = self->b->f81B4;
    if (f & 0x2000) {
        if (f & 0x40) {
            if (self->b->h9B0 != 0) {
                func_08B2BC40(self->b->h9B0);
            }
            if (self->b->h9B8 != 0) {
                func_08B2BC40(self->b->h9B8);
            }
            if (self->b->h8044 != 0) {
                func_08AAB8B4(self->b->h8044, 1);
            }
            if (self->b->h9C0 != 0) {
                func_08B2BC40(self->b->h9C0);
            }
            func_08A9AA84(self);
            self->b->s914 = 0x30;
            f = self->b->f81B4;
        }
        if (f & 0x80) {
            if (self->b->h8044 != 0) {
                func_08AAB8B4(self->b->h8044, 1);
            }
            if (self->b->h9C0 != 0) {
                func_08B2BC40(self->b->h9C0);
            }
        }
    }
}
