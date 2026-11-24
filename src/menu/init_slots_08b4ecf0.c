/* Clears word 0 and words 1..5, then sets four halfwords per 8-byte entry from +0x18 to 0xFFFF for 5 entries. */
void func_08B4ECF0(int *self)
{
    int i;
    int *p;
    unsigned short *h;
    unsigned short ff;
    self[0] = 0;
    i = 0;
    p = self;
    do { p[1] = 0; i++; p++; } while (i < 5);
    i = 0;
    ff = 0xFFFF;
    h = (unsigned short *)self;
    do {
        h[0x18 / 2] = ff;
        h[0x1A / 2] = ff;
        h[0x1C / 2] = ff;
        h[0x1E / 2] = ff;
        i++;
        h += 4;
    } while (i < 5);
}
