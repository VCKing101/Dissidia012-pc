/* Returns 1 when self->0x30 is null or has bit 0x20 set in its halfword at +0x20, else 0. */
struct Sub { char pad[0x20]; unsigned short flags; };
struct Self { char pad[0x30]; Sub *sub; };
struct T { static bool has(Sub *s) { return (s->flags & 0x20) != 0; } };
extern "C" int func_08B4C980(Self *self)
{
    if (self->sub == 0) { return 1; }
    else if (T::has(self->sub)) { return 1; }
    else { return 0; }
}
