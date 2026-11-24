/* When self->0x28 is nonzero, calls func_088D9F5C(self->0x30, 1, 1) and sets the halfword at +0xE of self->0x30 to 3. */
struct Sub { char pad[0xE]; short v; };
struct Self { char pad[0x28]; int on; int pad2; struct Sub *sub; };
extern void func_088D9F5C(struct Sub *s, int a, int b);
void func_08B530C4(struct Self *self)
{
    if (self->on == 0) { return; }
    func_088D9F5C(self->sub, 1, 1);
    self->sub->v = 3;
}
