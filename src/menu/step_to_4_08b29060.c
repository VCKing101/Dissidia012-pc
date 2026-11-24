/* After func_08B29538(self), if func_08B295B8(self) succeeds: func_08885220(D_089D69E8), func_088D3A04(self->0x14C + 4), set state 4 at +0x408 of self->0x14C. */
struct Sub { char pad[0x408]; int state; };
struct Self { char pad[0x14C]; struct Sub *sub; };
extern void *D_089D69E8;
extern void func_08B29538(struct Self *self);
extern int func_08B295B8(struct Self *self);
extern void func_08885220(void *p);
extern void func_088D3A04(char *p);
void func_08B29060(struct Self *self)
{
    func_08B29538(self);
    if (func_08B295B8(self) == 0) { return; }
    func_08885220(D_089D69E8);
    func_088D3A04((char *)self->sub + 4);
    self->sub->state = 4;
}
