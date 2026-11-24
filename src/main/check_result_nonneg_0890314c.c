/* Runs func_08903370, then returns whether func_089C5960 gave a non-negative value. */
extern void func_08903370(void);
extern int func_089C5960(void);
int func_0890314C(void)
{
    int r;
    func_08903370();
    r = func_089C5960();
    if (r < 0) {
        return 0;
    } else {
        return 1;
    }
}
