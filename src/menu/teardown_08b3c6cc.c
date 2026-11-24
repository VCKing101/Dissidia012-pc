/* Walks the list entries releasing each, then shuts down the shared objects. */
extern void *func_08899300(void);
extern void func_08898F84(void *a);
extern void func_08898F5C(void *a);
extern int *func_0881E75C(int *p);
extern void func_08844520(void *a, int b);
extern void func_0881E200(void *a);
extern void func_08893750(void *a);
extern void func_08893CEC(void *a);
extern void func_08893910(void *a);
extern void *D_089D4960;
extern void *D_089D4910;
extern void *D_089D6AC8;

void func_08B3C6CC(void)
{
    int *p;
    func_08898F84(func_08899300());
    p = 0;
    while ((p = func_0881E75C(p)) != 0) {
        int v = *p;
        func_08844520(D_089D4960, v);
    }
    func_0881E200(D_089D4910);
    func_08893750(D_089D6AC8);
    func_08893CEC(D_089D6AC8);
    func_08898F5C(func_08899300());
    func_08893910(D_089D6AC8);
}
