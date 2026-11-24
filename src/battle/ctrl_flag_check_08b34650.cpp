// Returns 1 when func_08B34378 reports true and mask 0x80000 is set in the flags word of the object at +0x5A4.
struct Ctrl08B34650 { char pad[0xC]; unsigned flags; };
struct Owner08B34650 {
    char pad[0x5A4];
    Ctrl08B34650 *ctrl;
    static bool Has(unsigned f, unsigned m) { return (f & m) != 0; }
};
extern "C" {
extern int func_08B34378(void *);
unsigned char func_08B34650(Owner08B34650 *self)
{
    int t = 0;
    if (func_08B34378(self) != 0) {
        if (Owner08B34650::Has(self->ctrl->flags, 0x80000)) {
            t = 1;
        }
    }
    return t;
}
}
