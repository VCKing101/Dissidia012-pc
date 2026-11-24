/* Calls func_08B52A24 when bit 0x20 is set in the halfword at +0x20 of self->0x38. */
struct Sub { char pad[0x20]; unsigned short flags; };
struct Self { char pad[0x38]; Sub *sub; };
extern "C" void func_08B52A24(void *self);
struct T { static bool has(Sub *s) { return (s->flags & 0x20) != 0; } };
extern "C" void func_08B529E4(Self *self)
{
    if (!T::has(self->sub)) { return; }
    func_08B52A24(self);
}
