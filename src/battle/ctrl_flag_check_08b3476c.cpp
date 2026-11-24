// Returns 1 when func_08B34378 reports true and mask 0x40000 is set in the flags word of the object at +0x5A4.
struct Ctrl08B3476C { char pad[0xC]; unsigned flags; };
struct Owner08B3476C {
    char pad[0x5A4];
    Ctrl08B3476C *ctrl;
    static bool Has(unsigned f, unsigned m) { return (f & m) != 0; }
};
extern "C" {
extern int func_08B34378(void *);
unsigned char func_08B3476C(Owner08B3476C *self)
{
    int t = 0;
    if (func_08B34378(self) != 0) {
        if (Owner08B3476C::Has(self->ctrl->flags, 0x40000)) {
            t = 1;
        }
    }
    return t;
}
}
