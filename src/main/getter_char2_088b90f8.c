// Returns the signed byte at offset 2 of the object func_088B9004 returns, or 0 if none.
extern char *func_088B9004(void);
int func_088B90F8(void)
{
    char *p = func_088B9004();
    if (p) return p[2];
    return 0;
}
