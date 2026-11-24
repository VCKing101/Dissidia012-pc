/* Stores a halfword at +0 of slot i (stride 0x168 base +0x1D1C). */
void func_08B43024(char *self, int i, int v)
{
    short *r = (short *)(self + i * 0x168 + 0x1D1C);
    r[0] = v;
}
