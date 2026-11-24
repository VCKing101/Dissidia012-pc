/* Open a connection through the helper with the context, store its handle on success; return 1 on success. */
extern int func_089C5A28(char *buf);
extern int func_089C59C8(char *buf, unsigned short a, int ctx, unsigned short a2, int b, int c, int d, int e);
int func_089063A0(int self, int a, int b)
{
    char buf[8];
    int h;
    *(int *)(self + 0x30) = 0;
    h = func_089C5A28(buf);
    if (h < 0) {
        return 0;
    }
    h = func_089C59C8(buf, a, self + 4, a, b, 500000, 0x1E, 0);
    if (h < 0) {
        return 0;
    }
    *(int *)(self + 0xC) = h;
    return 1;
}
