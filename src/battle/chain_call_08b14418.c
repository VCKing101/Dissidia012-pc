// Calls func_088908E0(self) and passes its result with a1, a2 and the sub-object at +0x4C to func_0887AC70.
extern int func_088908E0(void *);
extern void func_0887AC70(void *, int, int, int);
void func_08B14418(char *self, int a1, int a2)
{
    int b = a2;
    int a = a1;
    char *sub = self + 0x4C;
    int v = func_088908E0(self);
    func_0887AC70(sub, a, b, v);
}
