/* Stores a halfword at +0x12 of record i (stride 0x2E8, base +0xE94). */
void func_08B42F38(char *self, int i, int v)
{
    short *r = (short *)(self + i * 0x2E8 + 0xE94);
    r[9] = v;
}
