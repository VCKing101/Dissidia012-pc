/* For i in 0..4 calls func_08912AD8(func_08B36490(D_08B41CC0), i, D_08B41CC0->e[i].a), entries 8 bytes at +0x10. */
struct E { int a, b; };
struct Tab { char pad[0x10]; E e[5]; };
extern "C" Tab *D_08B41CC0;
extern "C" int func_08B36490(Tab *t);
extern "C" void func_08912AD8(int h, int i, int v);
extern "C" void func_08B35DC4(void)
{
    int h = func_08B36490(D_08B41CC0);
    for (int i = 0; i < 5; i++) {
        func_08912AD8(h, i, D_08B41CC0->e[i].a);
    }
}
