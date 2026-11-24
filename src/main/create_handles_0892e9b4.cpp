// Run the base setup, create the handle for the given source and tag it.
extern "C" void func_0892EA3C(int *self);
extern "C" int func_088DAA68(int src);
extern "C" void func_088D3CF0(int a, int *p, void *tbl);
extern "C" char D_089CEFF0[];
extern "C" void func_0892E9B4(int *self, int src)
{
    func_0892EA3C(self);
    for (int i = 0, *p = self; i < 1; i++, p++) {
        p[3] = func_088DAA68(src);
    }
    func_088D3CF0(0, self + 3, D_089CEFF0);
    *(short *)(self[3] + 0xE) = 1;
}
