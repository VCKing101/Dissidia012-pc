// If the handle at +0 has bit 5 of its short at +0x20 set, calls func_088D9CFC on it and clears the state at +4.
struct Handle08B41970 { char pad[0x20]; unsigned short flags; };
struct Obj08B41970 {
    Handle08B41970 *h;
    int state;
    static bool Has(unsigned f, unsigned m) { return (f & m) != 0; }
};
extern "C" {
extern void func_088D9CFC(void *);
void func_08B41970(Obj08B41970 *self)
{
    if (self->h != 0) {
        int one = 1;
        if (Obj08B41970::Has(self->h->flags, 0x20) == one) {
            func_088D9CFC(self->h);
            self->state = 0;
        }
    }
}
}
