/* Calls func_08844588 with D_089D4960, func_08933C00 with D_089D85E8, then func_089E0BDC(D_089E75C4, 0). */
extern void *D_089D4960;
extern void *D_089D85E8;
extern void *D_089E75C4;
extern void func_08844588(void *p);
extern void func_08933C00(void *p);
extern void func_089E0BDC(void *p, int v);
void func_08917A9C(void)
{

    func_08844588(D_089D4960);
    func_08933C00(D_089D85E8);
    void *obj = D_089E75C4;
    int zero = 0;
    func_089E0BDC(obj, zero);
}
