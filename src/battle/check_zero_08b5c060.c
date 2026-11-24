/* Returns whether func_08AB5558 on the global object gives zero. */
extern void *D_08AB6738;
extern int func_08AB5558(void *);
int func_08B5C060(void)
{
    int r = func_08AB5558(D_08AB6738);
    if (r) {
        return 0;
    } else {
        return 1;
    }
}
