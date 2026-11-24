/* After func_08B4E9A0(self): self+0x34 = func_088DAA68(self->0x2C); func_088D3CF0(0, self+0x34, func_08B4EC58); sets halfword +0xE of the new object to 4. */
struct Obj { char pad[0xE]; short v; };
struct Self { char pad[0x2C]; int src; char pad2[4]; Obj *obj; };
extern "C" void func_08B4E9A0(Self *self);
extern "C" int func_088DAA68(int src);
extern "C" void func_088D3CF0(int z, Obj **o, void *tag);
extern "C" char D_08B4EC58[];
extern "C" void func_08B4E924(Self *self)
{
    func_08B4E9A0(self);
    for (int i = 0, *p = (int *)self; i < 1; i++, p++) { p[0x34 / 4] = func_088DAA68(self->src); }
    func_088D3CF0(0, &self->obj, D_08B4EC58);
    self->obj->v = 4;
}
