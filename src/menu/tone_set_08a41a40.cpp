extern "C" {
extern void func_08A42CCC(void);
extern int func_088D4C5C(void *self);
extern void func_08873D34(int a, void (*callback)(void), float c, int d, int e, int f);
extern void func_08873DEC(int a, int b);
extern void func_088D54DC(void *o);
}

extern "C" void func_08A41A40(char *self)
{
    int level = func_088D4C5C(self);
    func_08873D34(3, func_08A42CCC, (float)level, 0, 1, 2);
    func_08873DEC(3, 2);
    func_088D54DC(self + 0x13C);
}
