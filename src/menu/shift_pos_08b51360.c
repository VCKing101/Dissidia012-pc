/* Sets bit 2 of the halfword at +0x20 of self->0x34 and calls func_088D9F08(sub->4 - n * D_08B51464, sub). */
struct Sub { char pad[4]; float y; char pad2[0x20 - 8]; unsigned short flags; };
struct Self { char pad[0x34]; struct Sub *sub; };
extern float D_08B51464;
extern void func_088D9F08(float v, struct Sub *s);
void func_08B51360(struct Self *self, int n)
{
    float d = (float)n * D_08B51464;
    float neg = -d;
    struct Sub *s = self->sub;
    float y = s->y;
    s->flags |= 2;
    func_088D9F08(neg + y, self->sub);
}
