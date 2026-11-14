/* Releases the child object and resets its owner.
   Retail keeps `self` in the frame (0($sp)) across the calls instead of an
   s-register. SNC -O2 never promotes a local object that has a constructor,
   so holding `self` in one (Kept) reproduces the store before the call and
   the reload after it. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Owner0883EBB0 {
    char unknown_00[0x20];
    void *child;
};

extern "C" {

void func_088A51E4(void *child);
void func_0883ECC4(Owner0883EBB0 *self);

void func_0883EBB0(Owner0883EBB0 *self)
{
    if (self->child != 0) {
        Kept<Owner0883EBB0 *> saved(self);
        func_088A51E4(self->child);
        func_0883ECC4(saved.value);
    }
}

}
