/* Menu page teardown: releases the sub object and the heap handles, hands the screen block its saved word and calls the stored callback. */
typedef struct Sub { char p[0x28]; int s28; } Sub;
typedef struct G { char p[0x1660]; int f1660; } G;
typedef struct O { char p0[0x134]; int heap; int h138; Sub *sub; void (*cb)(int); int cbarg; char p1[0x10]; int f158; } O;
extern void func_08B54CA4(Sub *);
extern void func_0887B1F4(void *, int);
extern void func_0885C538(int, Sub *, int);
extern void func_0885C488(int);
extern void func_0888208C(int);
extern G *D_089D8364;
void func_08B540D4(O *self) {
    if (self->sub != 0) {
        func_08B54CA4(self->sub);
        if (self->sub != 0) {
            func_0887B1F4(&self->sub->s28, 2);
        }
        func_0885C538(self->heap, self->sub, 0);
        self->sub = 0;
    }
    if (D_089D8364 != 0) {
        D_089D8364->f1660 = self->f158;
        self->f158 = 0;
    }
    func_0885C488(self->heap);
    func_0888208C(self->h138);
    self->heap = 0;
    self->h138 = 0;
    if (self->cb != 0) {
        self->cb(self->cbarg);
    }
}
