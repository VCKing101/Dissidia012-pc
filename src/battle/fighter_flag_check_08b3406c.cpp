// Returns 1 when bit 4 of the byte at +0x5B2 is set and bit 4 of byte 8 of the record at +0x2F8 is set.
struct Rec08B3406C { char pad[8]; unsigned char b; };
struct Flags08B3406C { char pad[2]; unsigned char b2; };
struct Owner08B3406C {
    char pad0[0x2F8];
    Rec08B3406C *rec;
    char pad1[0x5B0 - 0x2FC];
    Flags08B3406C fl;
    static bool Has(unsigned f, unsigned m) { return (f & m) != 0; }
};
extern "C" {
unsigned char func_08B3406C(Owner08B3406C *self)
{
    int t = 0;
    Flags08B3406C *f = &self->fl;
    if (Owner08B3406C::Has(f->b2, 4)) {
        if (Owner08B3406C::Has(self->rec->b, 4)) {
            t = 1;
        }
    }
    return t;
}
}
