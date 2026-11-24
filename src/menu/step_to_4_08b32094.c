/* After func_08B3261C(self), if func_08B3269C(self) succeeds: func_08885220(D_089D69E8), func_088D3A04(self->0x138), set state 4 at +0x400 of self->0x138. */
struct Sub { char pad[0x400]; int state; };
struct Self { char pad[0x138]; struct Sub *sub; };
extern void *D_089D69E8;
extern void func_08B3261C(struct Self *self);
extern int func_08B3269C(struct Self *self);
extern void func_08885220(void *p);
extern void func_088D3A04(struct Sub *p);
void func_08B32094(struct Self *self)
{
    func_08B3261C(self);
    if (func_08B3269C(self) == 0) { return; }
    func_08885220(D_089D69E8);
    func_088D3A04(self->sub);
    self->sub->state = 4;
}
