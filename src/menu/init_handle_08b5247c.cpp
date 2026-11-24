/* Releases old handles, creates a handle with func_088DAA68 from the id at +0x2C, registers it with func_088D3CF0 and sets its halfword at +0xE to 1. */
struct H { char pad[0xE]; short v; };
extern "C" void func_08B524F8(int *self);
extern "C" int func_088DAA68(int id);
extern "C" void func_088D3CF0(int a, int *slot, void (*fn)(void));
extern "C" void D_08B52598(void);
extern "C" void func_08B5247C(int *self)
{
    func_08B524F8(self);
    for (int i = 0, *p = self; i < 1; i++, p++) {
        p[0x30 / 4] = func_088DAA68(self[0x2C / 4]);
    }
    func_088D3CF0(0, self + 0x30 / 4, D_08B52598);
    ((H *)self[0x30 / 4])->v = 1;
}
