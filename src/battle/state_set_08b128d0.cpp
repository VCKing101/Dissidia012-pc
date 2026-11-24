// Sets state 2 (word at +0x88C of the inner object at +8): directly when the byte at +0x892 is zero, else after func_08873DEC(3, 2) when bit 5 of the flags of the record at +0x400 is set.
struct Rec08B128D0 { char pad[0x20]; unsigned short flags; };
struct In08B128D0 { char pad0[0x400]; Rec08B128D0 *rec; char pad1[0x88C - 0x404]; int state; char pad2[2]; unsigned char b892; };
struct Obj08B128D0 {
    char pad[8];
    In08B128D0 *in;
    static bool Has(unsigned f, unsigned m) { return (f & m) != 0; }
};
extern "C" {
extern void func_08873DEC(int, int);
void func_08B128D0(Obj08B128D0 *self)
{
    In08B128D0 *in = self->in;
    if (in->b892 != 0) {
        if (Obj08B128D0::Has(in->rec->flags, 0x20)) {
            func_08873DEC(3, 2);
            self->in->state = 2;
        }
    } else {
        in->state = 2;
    }
}
}
