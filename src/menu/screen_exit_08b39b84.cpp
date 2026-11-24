/* Tears down a screen: releases its widgets, drops the focus object when flag 0x2000 is clear. */
extern "C" {
void func_08B3C448(void);
void func_08B4E794(void *a);
void func_08B43E78(void);
void func_08B523FC(void *a);
void func_08931270(void *a);
void func_0892D380(void *a, void *b, int c, int d);
void func_08B342D4(void *a, int b);
}

struct W39B84 { void *w0; char pad04[0x10]; void *w14; char pad18[4]; void *w1C; char pad20[4]; void *w24; void *w28; };
struct B39B84 { char pad[0xC]; void *wC; char pad10[0x1C]; int f2C; };
struct S39B84 {
    char pad[0x44];
    unsigned flags;
    char pad48[0x7C];
    B39B84 *b;
    W39B84 *w;
    bool Has(unsigned m) { return (flags & m) != 0; }
};

extern "C" void func_08B39B84(S39B84 *self)
{
    func_08B3C448();
    func_08B4E794(self->w->w24);
    if (self->b->f2C != 0) {
        func_08B43E78();
    }
    func_08B523FC(self->w->w28);
    if (!self->Has(0x2000)) {
        func_08931270(self->w->w14);
    }
    func_0892D380(self->w->w1C, self->w->w0, 0xB, 1);
    func_08B342D4(self->b->wC, 1);
}
