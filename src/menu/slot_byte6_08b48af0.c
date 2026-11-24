/* If func_08B497F4 returns a non-negative index, returns the signed byte at +6 of func_088470B8(index); else 0. */
extern int func_08B497F4(void);
extern char *func_088470B8(int idx);
int func_08B48AF0(void)
{
    int r = 0;
    int idx = func_08B497F4();
    if (idx >= 0) { r = func_088470B8(idx)[6]; }
    return r;
}
