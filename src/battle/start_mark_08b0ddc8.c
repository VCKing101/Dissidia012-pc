// Runs func_08B0E4D4(self); if the flag byte is 1 also func_08B0E04C(self); then sets the byte at +0x9B.
extern void func_08B0E4D4(void *);
extern void func_08B0E04C(void *);
typedef struct { char pad[0x9B]; unsigned char mark; } Obj08B0DDC8;
void func_08B0DDC8(Obj08B0DDC8 *self, unsigned char flag)
{
    unsigned char f = flag;
    func_08B0E4D4(self);
    if (f == 1) {
        func_08B0E04C(self);
    }
    self->mark = 1;
}
