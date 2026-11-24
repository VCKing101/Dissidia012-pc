// Returns the signed byte at offset 2 of the object func_088B8FD0 returns, or 0 if none.
extern char *func_088B8FD0(void);
int func_088B90C4(void)
{
    char *p = func_088B8FD0();
    if (p) return p[2];
    return 0;
}
