/* Sets the state word of self->0xC4 to 10, reads two halfword values from its +0x10 object into 0x09B6DB54/56, runs the follow-up calls. */
struct Sub { int state; int pad; int pad2; int cam; int obj; };
struct Self { char pad[0xC4]; struct Sub *sub; };
struct Blk { char pad[0x94]; short a, b; };
extern struct Blk D_09B6DAC0;
extern short func_08B45F04(int o);
extern short func_08B45F24(int o);
extern void func_08B40590(struct Self *self);
extern void func_08B331F8(int cam);
extern void func_08B45E0C(int o);
void func_08B369C0(struct Self *self)
{
    self->sub->state = 10;
    D_09B6DAC0.a = func_08B45F04(self->sub->obj);
    D_09B6DAC0.b = func_08B45F24(self->sub->obj);
    func_08B40590(self);
    func_08B331F8(self->sub->cam);
    func_08B45E0C(self->sub->obj);
}
