/* Stores byte a at +0x12 of record i (stride 0x2E8, base +0xE94) and 2 or 1 at +0x13 depending on whether b is 1. */
void func_08B42FF0(char *self, int i, int a, int b)
{
    char *r = self + i * 0x2E8 + 0xE94;
    r[0x12] = a;
    r[0x13] = b == 1 ? 2 : 1;
}
