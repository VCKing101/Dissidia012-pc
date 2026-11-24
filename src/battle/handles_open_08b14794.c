// For each of the handles at +0xC, +0x10 and +0x30: starts it with func_088D9F5C and sets its short at +0xE; sets the byte at +0x60.
extern void func_088D9F5C(void *, int, unsigned char);
typedef struct { char pad[0xE]; short mode; } Handle08B14794;
typedef struct { char pad0[0xC]; Handle08B14794 *h0c; Handle08B14794 *h10; char pad1[0x30 - 0x14]; Handle08B14794 *h30; char pad2[0x60 - 0x34]; unsigned char b60; } Obj08B14794;
void func_08B14794(Obj08B14794 *self)
{
    if (self->h0c != 0) {
        func_088D9F5C(self->h0c, 0, 1);
        self->h0c->mode = 1;
    }
    if (self->h10 != 0) {
        func_088D9F5C(self->h10, 6, 1);
        self->h10->mode = 7;
    }
    if (self->h30 != 0) {
        func_088D9F5C(self->h30, 0, 1);
        self->h30->mode = 1;
    }
    self->b60 = 1;
}
