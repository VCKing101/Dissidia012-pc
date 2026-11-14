extern unsigned char D_09AF87F0[];
extern void *D_089D4778;
extern int D_089D477C;

void func_08935524(void *pool, int a, int b);

void func_0880BE34(int a, int b) {
    void *pool = D_09AF87F0;
    void *zero = 0;
    func_08935524(pool, a, b);
    D_089D4778 = pool != 0 ? pool : zero;
    D_089D477C = 0;
}
