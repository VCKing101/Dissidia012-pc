// Returns 1 when func_08B34378 reports true and mask 0x200 is set in the flags word of the object at +0x5A4.
struct Ctrl08B3442C { char pad[0xC]; unsigned flags; };
struct Owner08B3442C {
    char pad[0x5A4];
    Ctrl08B3442C *ctrl;
    static bool Has(unsigned f, unsigned m) { return (f & m) != 0; }
};
extern "C" {
extern int func_08B34378(void *);
unsigned char func_08B3442C(Owner08B3442C *self)
{
    int t = 0;
    if (func_08B34378(self) != 0) {
        if (Owner08B3442C::Has(self->ctrl->flags, 0x200)) {
            t = 1;
        }
    }
    return t;
}
}
