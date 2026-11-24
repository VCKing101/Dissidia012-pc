/* If func_08885608(D_089D69E8, 0) succeeds: func_08885220(D_089D69E8), func_088D3A04(self->0x14C + 4), set state 7 at +0x408 of self->0x14C. */
struct Sub { char pad[0x408]; int state; };
struct Self { char pad[0x14C]; struct Sub *sub; };
extern void *D_089D69E8;
extern int func_08885608(void *p, int z);
extern void func_08885220(void *p);
extern void func_088D3A04(char *p);
void func_08B2915C(struct Self *self)
{
    void *g;
    int zero;
    g = D_089D69E8;
    zero = 0;
    if (func_08885608(g, zero) == 0) { return; }
    func_08885220(D_089D69E8);
    func_088D3A04((char *)self->sub + 4);
    self->sub->state = 7;
}
