/* When self->0x28 is nonzero, calls func_088D9F5C(self->0x30, 4, 0). */
struct Self { char pad[0x28]; int on; int pad2; void *sub; };
extern void func_088D9F5C(void *s, int a, int b);
void func_08B53110(struct Self *self)
{
    if (self->on == 0) { return; }
    func_088D9F5C(self->sub, 4, 0);
}
