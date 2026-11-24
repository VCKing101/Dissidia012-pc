// Thread entry: sets up, creates and starts the worker, then pumps the manager until it reports done.
extern void func_08993244();
extern int func_089C5F80(const void *, void *, int);
extern void func_089C5EF8(int);
extern void func_089C5F88(int, int);
extern void func_088769CC();
extern void func_0893354C(int);
extern int func_08933458(int);
extern void func_0893347C(int);
extern void func_08933378();
extern char D_089CF494[];
extern int D_089D85BC;
int func_08933398() {
    int t;
    int m;
    int r;
    func_08993244();
    t = func_089C5F80(D_089CF494, func_08933378, 0);
    func_089C5EF8(t);
    func_089C5F88(0, 0x4000);
    func_088769CC();
    m = D_089D85BC;
    func_0893354C(D_089D85BC);
    do {
        r = func_08933458(m);
        if (r == 0) break;
    } while (1);
    func_0893347C(m);
    return 0;
}
