/* Calls func_08920130 on work+0x3F548, then func_08B0CAD0 and func_08B05024 on self. */
extern char *D_089D712C;
extern void func_08920130(char *p, int unused);
extern void func_08B0CAD0(void *self);
extern void func_08B05024(void *self);
void func_08B0D31C(void *self, int x)
{
    func_08920130(D_089D712C + 0x3F548, x);
    func_08B0CAD0(self);
    func_08B05024(self);
}
