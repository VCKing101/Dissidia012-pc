/* Battle teardown: releases the global resources, then walks the resource list handing each entry back. */
extern int D_089D6AE0;
extern int D_08B54330;
extern int D_08ABBDF8;
extern int D_089D4960;
extern int D_089D4910;
extern int D_08AAAD80;
extern void func_08895AE4(int v);
extern void func_0888E4A0(void);
extern void func_08B533E4(int v);
extern void func_08ABAE40(int v);
extern int *func_0881E75C(int *prev);
extern void func_08844520(int a, int b);
extern void func_0881E200(int v);
extern void func_08AA7850(int v);
void func_08B13B9C(void)
{
    int *p;
    func_08895AE4(D_089D6AE0);
    func_0888E4A0();
    func_08B533E4(D_08B54330);
    func_08ABAE40(D_08ABBDF8);
    p = 0;
    while ((p = func_0881E75C(p)) != 0) {
        int v = *p;
        func_08844520(D_089D4960, v);
    }
    func_0881E200(D_089D4910);
    func_08AA7850(D_08AAAD80);
}
