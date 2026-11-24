// Calls func_08B4967C with all its arguments, then func_08B49764(self, a3, a4, a5) and sets the word at +0 to 1.
extern void func_08B4967C(int *, int, int, int, int, int);
extern void func_08B49764(int *, int, int, int);
void func_08B49290(int *self0, int a1, int a2, int a30, int a40, int a50)
{
    int a5 = a50;
    int a4 = a40;
    int *self = self0;
    int a3 = a30;
    func_08B4967C(self, a1, a2, a3, a4, a5);
    func_08B49764(self, a3, a4, a5);
    *self = 1;
}
