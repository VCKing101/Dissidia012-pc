/* Marks record i (stride 0x2E8, base +0xE94) with halfword 1 at +0 and stores a halfword at +0x10. */
void func_08B42EDC(char *self, int i, int v)
{
    short *r = (short *)(self + i * 0x2E8 + 0xE94);
    r[0] = 1;
    r[8] = v;
}
