/* Starts a load through the global loader, keeps the handle, then registers it with func_0887AC70 on D_09B6C2B0. */
extern void *D_089D7830;
extern int D_08B125C8;
extern char D_09B6C2B0[];
extern int func_088CF188(void *loader, int a, int b, int c, int d, int e, int f);
extern int func_088908E0(void);
extern void func_0887AC70(void *obj, int a, int b, int c);
void func_08B124D0(int a, int b, int c, int d)
{
    D_08B125C8 = func_088CF188(D_089D7830, a, b, 0, 0, 0, 0);
    func_0887AC70(D_09B6C2B0, c, d, func_088908E0());
}
