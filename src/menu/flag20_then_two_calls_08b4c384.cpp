/* If bit 0x20 of the halfword at +0x20 of self->0x30 is set, calls func_088D9F5C(sub, 0x1A, 1) and func_088D9CD8(self->0x34). */
struct Sub { char pad[0x20]; unsigned short flags; };
struct Self { char pad[0x30]; Sub *sub; void *other; };
extern "C" void func_088D9F5C(Sub *s, int a, int b);
extern "C" void func_088D9CD8(void *o);
struct T { static bool has(Sub *s) { return (s->flags & 0x20) != 0; } };
extern "C" void func_08B4C384(Self *self)
{
    if (T::has(self->sub)) {
        func_088D9F5C(self->sub, 0x1A, 1);
        func_088D9CD8(self->other);
    }
}
