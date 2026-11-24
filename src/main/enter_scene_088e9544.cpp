/* Enter the scene: reset the menu block, queue its two items, set the mode flag and update the helpers. */
extern "C" int D_089D6AE8;
extern "C" void func_088E9704(int self, int v);
extern "C" void func_088D727C(int p);
extern "C" void func_088D73B4(int p);
extern "C" void func_088D72E8(int p, int a, int b, int c, int d, int e);
extern "C" void func_088D73F4(int p, int v);
extern "C" int func_088729A0(int v);
extern "C" void func_088D743C(int p, int a, int b, int c, int d);
extern "C" void func_088CE72C(int p);
extern "C" void func_088A28E8(int a, int b);
extern "C" void func_088965F4(int a, int b);
extern "C" void func_088A3F2C(int a);
extern "C" void func_088E699C(int p);
extern "C" void func_088EA4D8(int p);
struct E {
    static bool nz(int c) { return c != 0; }
};
extern "C" void func_088E9544(int self)
{
    int blk;
    int h;
    func_088E9704(self, 2);
    blk = self + 0x1874;
    func_088D727C(blk);
    func_088D73B4(blk);
    func_088D72E8(blk, 0x21, -1, 0, 0, 0);
    if (*(int *)(self + 0x1864) == 4) {
        func_088D72E8(blk, 0x3A, -1, 0, 0, 0);
    } else {
        func_088D72E8(blk, 0x20, -1, 0, 0, 0);
    }
    func_088D73F4(blk, 1);
    h = func_088729A0(0);
    func_088D743C(blk, h, 0, 0, 0);
    func_088CE72C(self + 0x1AF0);
    func_088A28E8(0, 1);
    func_088965F4(D_089D6AE8, 1);
    func_088A3F2C(1);
    func_088E699C(self + 0x1340);
    if (E::nz(*(char *)(self + 0x13F0))) {
        func_088EA4D8(self + 0x13EC);
    }
}
