/* Stores byte a at +0x12 of record i (stride 0x2E8, base +0xC), and byte b at +0x13 unless b is -1. */
void func_08B42E48(char *self, int i, int a, int b)
{
    char *r = self + i * 0x2E8 + 0xC;
    if (b == -1) {
        r[0x12] = a;
    } else {
        r[0x12] = a;
        r[0x13] = b;
    }
}
