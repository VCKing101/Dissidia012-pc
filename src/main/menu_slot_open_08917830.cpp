struct W08917830 { char pad[0x60]; int v60; int v64; char pad1[0x574 - 0x68]; char blk[1]; char pad2[0x768 - 0x575]; int v768; char pad3[0x7DB - 0x76C]; unsigned char b7db; };
extern W08917830 *D_089D83D8;
extern int D_089D8508;
extern int D_089D6A10;
extern "C" short *func_0891705C(int);
extern "C" void func_08B448A0(void *, short *, int, int, int, int, int, void *);
extern "C" void func_0891B8C0(int, void *);
extern "C" void func_0891B96C(int, int, int);
extern "C" void func_0888964C(int, int, int, int, int);
extern "C" void func_08889ED4(int, void *, int);
extern "C" void func_08889808(int);
extern "C" void func_08917BF4();
extern "C" void func_08917C34();
extern "C" void func_08917C70();
extern "C" void func_08917830()
{
    char buf[0x170];
    int g = D_089D8508;
    int mode = -1;
    if (D_089D83D8->b7db != 0) mode = 1;
    short *ps0 = func_0891705C(-1);
    int n = 0;
    int i = 0;
    short *ps = ps0;
    do {
        if (*ps > 0) { } else { break; }
        i++; n++; ps++;
    } while (i < 5);
    func_08B448A0(buf, func_0891705C(-1), n, D_089D83D8->v60, D_089D83D8->v64, D_089D83D8->v768, mode, D_089D83D8->blk);
    func_0891B8C0(g, buf);
    func_0891B96C(g, 0, 0);
    func_0888964C(D_089D6A10, 1, 0, 3, 0);
    func_08889ED4(0, (void *)func_08917BF4, 0);
    func_08889ED4(1, (void *)func_08917C34, 0);
    func_08889ED4(2, (void *)func_08917C70, 0);
    func_08889808(D_089D6A10);
}
