/* Returns 1 unless self->0x30 is set, has kind 0 and bit 0x20 clear in its halfword at +0x20. */
struct Sub { char pad[0xC]; short kind; char pad2[0x20 - 0xE]; unsigned short flags; };
struct Self { char pad[0x30]; Sub *sub; };
struct T { static bool has(Sub *s) { return (s->flags & 0x20) != 0; } };
extern "C" int func_08B537E0(Self *self)
{
    Sub *s = self->sub;
    if (s != 0) {
        if (s->kind == 0) {
            if (!T::has(s)) { return 0; }
        }
    }
    return 1;
}
