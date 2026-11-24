/* Reads two halfword values from the +0x10 object of self->0xC4 into 0x09B6DB54/56, then calls func_08B39C9C(self, 0). */
struct Sub { char pad[0x10]; int obj; };
struct Self { char pad[0xC4]; struct Sub *sub; };
struct Blk { char pad[0x94]; short a, b; };
extern struct Blk D_09B6DAC0;
extern short func_08B45F04(int o);
extern short func_08B45F24(int o);
extern void func_08B39C9C(struct Self *self, int z);
void func_08B39870(struct Self *self)
{
    D_09B6DAC0.a = func_08B45F04(self->sub->obj);
    D_09B6DAC0.b = func_08B45F24(self->sub->obj);
    func_08B39C9C(self, 0);
}
