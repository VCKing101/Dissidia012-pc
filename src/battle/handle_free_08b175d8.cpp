// Runs func_08B17518, then if bit 5 of the short at +0x20 of the handle at +0x1C is set, frees it with func_088DA060 and clears the handle and the word at +0x28.
struct Handle08B175D8 { char pad[0x20]; unsigned short flags; };
struct Obj08B175D8 {
    char pad0[0x1C];
    Handle08B175D8 *h;
    char pad1[0x28 - 0x20];
    int w28;
    static bool Has(unsigned f, unsigned m) { return (f & m) != 0; }
};
extern "C" {
extern void func_08B17518(void *);
extern void func_088DA060(void *);
void func_08B175D8(Obj08B175D8 *self)
{
    func_08B17518(self);
    if (Obj08B175D8::Has(self->h->flags, 0x20) == 1) {
        func_088DA060(self->h);
        self->h = 0;
        self->w28 = 0;
    }
}
}
