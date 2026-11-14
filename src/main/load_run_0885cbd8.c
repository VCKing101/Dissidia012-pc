/* Starts a load with the completion callback, runs it and returns its result. */
extern unsigned char D_089D4E30;
extern int D_09B17DA8;

void func_0885CD08(void);
void func_088629DC(int a, int b, int c, void (*done)(void), int zero, unsigned char flags);
void func_0885CC70(void);

int func_0885CBD8(int a, int b, int c, unsigned char flags)
{
    D_089D4E30 = 0;
    D_09B17DA8 = 0;
    func_088629DC(a, b, c, func_0885CD08, 0, flags);
    func_0885CC70();
    return D_09B17DA8;
}
