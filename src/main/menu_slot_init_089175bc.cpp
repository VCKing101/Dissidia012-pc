struct T089175BC { int f0; int a[5]; int b[5]; int rest[40 - 5 + 0]; int x; int y; };
struct W089175BC { char pad[0x60]; int v60; int v64; char pad2[0x750 - 0x68]; int v750; char pad3[0x7CC - 0x754]; int v7cc; };
extern W089175BC *D_089D83D8;
extern int D_089D85E8;
extern int D_08B01CF8;
extern "C" int func_0891705C(int);
extern "C" int func_08912AE8(int, int);
extern "C" void func_08AD0A94();
extern "C" void func_08933C00(int);
extern "C" void func_08AF2F14(int);
extern "C" void func_08AF2F1C(int, int, void *, int, void *, int, int, int);
extern "C" void func_08865184(void *);
extern "C" int func_089176B0(void);
extern "C" void func_089175BC()
{
    T089175BC s;
    int h;
    int i0 = 0; int *p0 = (int *)&s;
    do { p0[1] = 0; p0[6] = 0; i0++; p0++; } while (i0 < 5);
    s.f0 = 0;
    h = func_0891705C(-1);
    { int i; int *p; for (i = 0, p = (int *)&s; i < 5; i++, p++) { p[1] = func_08912AE8(h, i); p[6] = 0; } }
    s.x = D_089D83D8->v60;
    s.y = D_089D83D8->v64;
    func_08AD0A94();
    func_08933C00(D_089D85E8);
    func_08AF2F14(D_08B01CF8);
    func_08AF2F1C(D_08B01CF8, D_089D83D8->v7cc, &s, 3, (void *)func_089176B0, D_089D83D8->v750, 1, 1);
    func_08865184((char *)D_089D83D8 + 4);
}
