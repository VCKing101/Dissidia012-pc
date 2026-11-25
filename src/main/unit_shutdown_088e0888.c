/* Shuts a unit down when its two flags are set: stops sound, frees queued handles and shared managers. */
extern int D_089D4960;
extern int D_089D4910;
extern int D_089D6AC8;
extern int D_089D4DC8;
extern int D_089D4D80;
extern int D_08B54330;
extern int func_08899300(void);
extern void func_08898F84(int);
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
extern void func_088E8738(int);
void func_088E0888(unsigned char *self)
{
    if (self[0x17B2] != 0) {
        func_08898F84(func_08899300());
        self[0x17B2] = 0;
    }
    if (self[0x17B1] != 0) {
        int *p = 0;
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
        func_088E8738((int)(self + 0x1270));
        self[0x17B1] = 0;
    }
}
