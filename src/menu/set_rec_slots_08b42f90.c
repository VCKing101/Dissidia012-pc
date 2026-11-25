/* Marks record i (stride 0x2E8, base +0xE94) with halfword 9, clears three halfwords at +0x10 to -1 and stores a at +0x10. */
void func_08B42F90(char *self, int i, int a)
{
    char *r = self + i * 0x2E8 + 0xE94;
    short *s = (short *)(r + 0x10);
    *(short *)r = 9;
    s[0] = -1;
    s[1] = -1;
    s[2] = -1;
    *(short *)(r + 0x10) = a;
}
