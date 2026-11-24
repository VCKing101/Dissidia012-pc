/* After func_08B50FCC(self), for two slots self+0x30 = func_088DAA68(self->0x2C); func_088D3CF0(0, self+0x30, func_08B51468); sets halfword +0xE of the first to 3. */
struct Obj { char pad[0xE]; short v; };
struct Self { char pad[0x2C]; int src; Obj *obj[2]; };
extern "C" void func_08B50FCC(Self *self);
extern "C" int func_088DAA68(int src);
extern "C" void func_088D3CF0(int z, Obj **o, void *tag);
extern "C" char D_08B51468[];
extern "C" void func_08B50F50(Self *self)
{
    func_08B50FCC(self);
    for (int i = 0, *p = (int *)self; i < 2; i++, p++) { p[0x30 / 4] = func_088DAA68(self->src); }
    func_088D3CF0(0, self->obj, D_08B51468);
    self->obj[0]->v = 3;
}
