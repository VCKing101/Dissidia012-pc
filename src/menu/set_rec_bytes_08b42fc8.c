/* Clears halfword +0 of record i (stride 0x2E8, base +0xE94), stores bytes at +0x10 and 1 at +0x11. */
void func_08B42FC8(char *self, int i, int v)
{
    char *r = self + i * 0x2E8 + 0xE94;
    *(short *)r = 0;
    r[0x10] = v;
    r[0x11] = 1;
}
