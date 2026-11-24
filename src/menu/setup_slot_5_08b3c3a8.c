/* Calls func_08B3C448(self), stores func_08B421FC(D_08B42288, 5) at +0xC of self->0xD0, then func_08B3C3FC(self, id). */
struct Sub { char pad[0xC]; int h; };
struct Self { char pad[0xD0]; struct Sub *sub; };
extern int D_08B42288;
extern void func_08B3C448(struct Self *self);
extern int func_08B421FC(int a, int n);
extern void func_08B3C3FC(struct Self *self, int id);
void func_08B3C3A8(struct Self *self, int id)
{
    func_08B3C448(self);
    self->sub->h = func_08B421FC(D_08B42288, 5);
    func_08B3C3FC(self, id);
}
