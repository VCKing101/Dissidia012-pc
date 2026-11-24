/* When func_08B249EC(self) says so and func_0887E718(D_089D6998) is zero, calls the 0x08865158 hook on self + 0x100. */
extern void *D_089D6998;
extern int func_08B249EC(void *self);
extern int func_0887E718(void *p);
extern void func_08865158(char *p);
void func_08B2461C(char *self)
{
    if (func_08B249EC(self) == 0) { return; }
    if (func_0887E718(D_089D6998) != 0) { return; }
    func_08865158(self + 0x100);
}
