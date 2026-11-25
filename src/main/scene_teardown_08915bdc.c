/* Tears down a scene: stops the sound state, frees the queued handles and the shared managers. */
extern int D_089D4960;
extern int D_089D4910;
extern int D_089D6AC8;
extern int D_089D4DC8;
extern int D_089D4D80;
extern int D_08B54330;
extern int func_08899300(void);
extern void func_08898F84(int);
extern void func_08898F5C(int);
extern int *func_0881E75C(int *);
extern void func_08844520(int, int);
extern void func_0881E200(int);
extern void func_08893750(int);
extern void func_08893CEC(int);
extern void func_08B532AC(int);
extern void func_08B533E4(int);
extern void func_08B53354(int);
extern void func_0885A31C(int);
extern void func_0884BE10(int);
extern void func_08910A04(int);
extern void func_08893910(int);
void func_08915BDC(int self)
{
    int *p;
    func_08898F84(func_08899300());
    p = 0;
    while ((p = func_0881E75C(p)) != 0) {
        int x = *p;
        func_08844520(D_089D4960, x);
    }
    func_0881E200(D_089D4910);
    func_08893750(D_089D6AC8);
    func_08893CEC(D_089D6AC8);
    func_08B532AC(D_08B54330);
    func_08B533E4(D_08B54330);
    func_08B53354(D_08B54330);
    func_0885A31C(D_089D4DC8);
    func_0884BE10(D_089D4D80);
    func_08910A04(self + 0x7C);
    func_08898F5C(func_08899300());
    func_08893910(D_089D6AC8);
}
