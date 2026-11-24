/* Run the base setup, create one handle from the arguments and register it. */
extern void func_0892EB10(void);
extern int func_0887B3EC(int a, int b, int c, int d, int e, int f);
extern void func_088D428C(int *a, int *b, void *tbl, int n);
extern char D_089CF000[];
void func_0892EA90(int *self, int a, int b, int c)
{
    int *o = self;
    int x = a;
    int h;
    func_0892EB10();
    h = func_0887B3EC(x, b, 0, c, 0, 0);
    o[4] = h;
    func_088D428C(o + 3, o + 4, D_089CF000, 1);
}
