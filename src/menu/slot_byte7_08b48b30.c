/* If func_08B497F4 returns a non-negative index, returns the signed byte at +7 of func_088470B8(index); else -1. */
extern int func_08B497F4(void);
extern char *func_088470B8(int idx);
int func_08B48B30(void)
{
    int r = -1;
    int idx = func_08B497F4();
    if (idx >= 0) { r = func_088470B8(idx)[7]; }
    return r;
}
