/* Open a connection through the helper, store its handle on success; return 1 on success. */
extern int func_089C5A28(char *buf);
extern int func_089C5A10(char *buf, unsigned short a, int b, int c, int d, int e, int f);
int func_08905D48(int self, int a, int b)
{
    char buf[8];
    int h = func_089C5A28(buf);
    if (h < 0) {
        return 0;
    }
    h = func_089C5A10(buf, a, b, 500000, 0x1E, 5, 0);
    if (h < 0) {
        return 0;
    }
    *(int *)(self + 0x2C8) = h;
    return 1;
}
