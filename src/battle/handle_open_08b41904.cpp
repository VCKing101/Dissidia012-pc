// If the handle at +0 has bit 5 of its short at +0x20 set, calls func_088D9F5C(h, 2, 1) and sets the state at +4 to 2.
struct Handle08B41904 { char pad[0x20]; unsigned short flags; };
struct Obj08B41904 {
    Handle08B41904 *h;
    int state;
    static bool Has(unsigned f, unsigned m) { return (f & m) != 0; }
};
extern "C" {
extern void func_088D9F5C(void *, int, int);
void func_08B41904(Obj08B41904 *self)
{
    if (self->h != 0) {
        int one = 1;
        if (Obj08B41904::Has(self->h->flags, 0x20) == one) {
            int two = 2;
            func_088D9F5C(self->h, 2, 1);
            self->state = two;
        }
    }
}
}
