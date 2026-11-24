/* After func_08B29538(self), if func_08B295B8(self) succeeds: func_08B292F4(self), func_088853CC(D_089D69E8, 0), set state 6 at +0x408 of self->0x14C. */
struct Sub { char pad[0x408]; int state; };
struct Self { char pad[0x14C]; struct Sub *sub; };
extern void *D_089D69E8;
extern void func_08B29538(struct Self *self);
extern int func_08B295B8(struct Self *self);
extern void func_08B292F4(struct Self *self);
extern void func_088853CC(void *p, int z);
void func_08B290FC(struct Self *self)
{
    func_08B29538(self);
    if (func_08B295B8(self) == 0) { return; }
    func_08B292F4(self);
    void *g;
    int zero;
    g = D_089D69E8;
    zero = 0;
    func_088853CC(g, zero);
    self->sub->state = 6;
}
