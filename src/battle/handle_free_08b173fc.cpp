// If bit 5 of the short at +0x20 of the handle at +0x14 is set, frees it with func_088DA060 and clears the handle and the word at +0x28.
struct Handle08B173FC { char pad[0x20]; unsigned short flags; };
struct Obj08B173FC {
    char pad0[0x14];
    Handle08B173FC *h;
    char pad1[0x28 - 0x18];
    int w28;
    static bool Has(unsigned f, unsigned m) { return (f & m) != 0; }
};
extern "C" {
extern void func_088DA060(void *);
void func_08B173FC(Obj08B173FC *self)
{
    if (Obj08B173FC::Has(self->h->flags, 0x20) == 1) {
        func_088DA060(self->h);
        self->h = 0;
        self->w28 = 0;
    }
}
}
