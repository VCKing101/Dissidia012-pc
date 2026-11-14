extern unsigned char D_089D4E30;
extern int D_09B17DA8;
extern void func_0885CD2C(void);
extern void func_08862D6C(int a, void (*fn)(void), int b, int c);
extern void func_0885CC70(void);
int func_0885CC24(int a)
{
    D_089D4E30 = 0;
    D_09B17DA8 = 0;
    func_08862D6C(a, func_0885CD2C, 0, 1);
    func_0885CC70();
    return D_09B17DA8;
}
