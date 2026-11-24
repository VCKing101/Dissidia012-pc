/* If func_089C5948 is non-negative, calls func_08903370(a, b, c) and returns 1; else 0. */
extern int func_089C5948(void);
extern void func_08903370(int a, int b, int c);
int func_08902DDC(int a, int b, int c)
{
    if (func_089C5948() < 0) {
        return 0;
    } else {
        func_08903370(a, b, c);
        return 1;
    }
}
