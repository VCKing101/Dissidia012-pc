/* Forwards its arguments to func_08B49B48 with the fourth zeroed and the old fourth as a signed byte in the fifth. */
extern void func_08B49B48(int a, int b, int c, int d, char e);
void func_08B49B24(int a, int b, int c, int d)
{
    int cell[1];
    cell[0] = d;
    func_08B49B48(a, b, c, 0, *(char *)cell);
}
