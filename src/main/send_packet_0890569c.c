/* Send up to 0xFFF3 bytes through the transport handle; return 1 on success. */
extern int func_089C59F0(int h, int a, int b, int c, int d, int e, int f);
int func_0890569C(int *self, int a, int b, int n)
{
    int r;
    if (0xFFF3 < n) {
        return 0;
    }
    r = func_089C59F0(self[0], a, self[1] & 0xFFFF, b, n, 3000000, 0);
    if (r < 0) {
        return 0;
    }
    return 1;
}
