/* Runs a setup call, forwards its parameters to the init routine and marks the object ready. */
extern void func_08B4CBE8(int *self, int a1, int a2, int a3);
extern void func_08B4CD3C(int *self, int a, int b, int c, int d, int e, int f, int g, unsigned char h);

void func_08B4CB30(int *self, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, unsigned char a11)
{
    func_08B4CBE8(self, a1, a2, a3);
    func_08B4CD3C(self, a4, a5, a6, a7, a8, a9, a10, a11);
    *self = 1;
}
