/* Calls func_08992F04(p) when p is non-null and bit 0 of flags is set. */
extern void func_08992F04(void *p);
void func_08B58B38(void *p, unsigned flags)
{
    if (p != 0 && (flags & 1) != 0) { func_08992F04(p); }
}
