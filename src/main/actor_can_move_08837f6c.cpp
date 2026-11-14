/* Whether an actor can take a move: it must be idle and not dead, not
   frozen or grabbed; then a forced state or being visible allows it. */
extern "C" {
extern unsigned long long D_089C7EF8;
extern unsigned long long D_089C7FD0;
}

struct Actor08837F6C {
    char pad_000[0x380];
    unsigned int flags;
    char pad_384[4];
    unsigned long long state;

    static bool any(Actor08837F6C *a, unsigned long long mask) { return (a->state & mask) != 0; }
    static bool idle(Actor08837F6C *a) { return !any(a, D_089C7EF8); }
    static bool flag(Actor08837F6C *a, unsigned int mask) { return (a->flags & mask) != 0; }
    static bool visible(Actor08837F6C *a) { return !flag(a, 0x4000000); }
};

extern "C" {

int func_08837990(Actor08837F6C *self);

bool func_08837F6C(Actor08837F6C *self)
{
    if (!Actor08837F6C::idle(self) || func_08837990(self)) {
        return false;
    }
    if (Actor08837F6C::flag(self, 0x800000)) {
        return false;
    }
    if (Actor08837F6C::flag(self, 0x200000)) {
        return false;
    }
    if (!Actor08837F6C::any(self, D_089C7FD0) && !Actor08837F6C::visible(self)) {
        return false;
    }
    return true;
}

}
