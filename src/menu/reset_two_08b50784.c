/* Calls func_088D9F5C(self->0x30, 3, 0) if set; if self->0x34 is set calls func_088D9F5C(it, 1, 0) and clears bit 2 of its halfword at +0x20. */
struct Sub { char pad[0x20]; unsigned short flags; };
struct Self { char pad[0x30]; void *a; struct Sub *b; };
extern void func_088D9F5C(void *s, int a, int b);
void func_08B50784(struct Self *self)
{
    if (self->a != 0) { func_088D9F5C(self->a, 3, 0); }
    if (self->b != 0) {
        func_088D9F5C(self->b, 1, 0);
        self->b->flags = self->b->flags & ~2;
    }
}
