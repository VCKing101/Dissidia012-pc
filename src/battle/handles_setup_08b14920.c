// Starts the handles at +0x20, +0x14, +0x18, +0x1C, +0x24, +0x28, +0x2C with func_088D9F5C (modes depend on whether the record at +0x5C is set), then sets the byte at +0x61.
extern void func_088D9F5C(void *, int, unsigned char);
typedef struct { char pad[8]; short m8; short ma; } Rec08B14920;
typedef struct {
    char pad0[0x14];
    void *h14, *h18, *h1c, *h20, *h24, *h28, *h2c;
    char pad1[0x5C - 0x30];
    Rec08B14920 *rec;
    char pad2;
    unsigned char b61;
} Obj08B14920;
void func_08B14920(Obj08B14920 *self)
{
    func_088D9F5C(self->h20, 0xE, 1);
    if (self->rec != 0) {
        func_088D9F5C(self->h14, 9, 1);
        func_088D9F5C(self->h18, 0xA, 1);
        func_088D9F5C(self->h1c, 0xA, 1);
        func_088D9F5C(self->h24, self->rec->m8, 1);
        func_088D9F5C(self->h28, self->rec->ma, 1);
        func_088D9F5C(self->h2c, self->rec->ma, 1);
    } else {
        func_088D9F5C(self->h24, 0, 1);
        func_088D9F5C(self->h28, 1, 1);
        func_088D9F5C(self->h2c, 1, 1);
    }
    self->b61 = 1;
}
