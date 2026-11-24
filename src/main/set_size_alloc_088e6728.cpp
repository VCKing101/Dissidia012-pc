/* Stores a size (0x280000 if flag is set, else 0x100000) in field 0xC, then the func_0886658C result for it in field 8. */
typedef struct Obj088e6728 { unsigned char pad[8]; void *mem; unsigned size; } Obj088e6728;
extern "C" void *func_0886658C(unsigned size);
extern "C" void func_088E6728(Obj088e6728 *self, unsigned char big)
{
    Obj088e6728 *keep[1];
    unsigned size = big ? 0x280000 : 0x100000;
    keep[0] = self;
    self->size = size;
    void *m = func_0886658C(size);
    self = keep[0];
    self->mem = m;
}
