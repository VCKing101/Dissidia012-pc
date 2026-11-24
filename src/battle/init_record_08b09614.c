// Initialises the record (-1 / 0 fields, 0x18-byte block at +0x10 cleared); returns it.
extern void func_08935828(void *, int, int);
typedef struct { int a; unsigned char b; char pad0[3]; int c; int d; char pad1[0x28 - 0x10]; int e, f, g; } Rec08B09614;
Rec08B09614 *func_08B09614(Rec08B09614 *self)
{
    int neg = -1;
    self->a = neg;
    self->b = 0;
    self->c = neg;
    self->d = 0;
    self->e = 0;
    self->f = 0;
    self->g = 0;
    func_08935828((char *)self + 0x10, 0, 0x18);
    return self;
}
