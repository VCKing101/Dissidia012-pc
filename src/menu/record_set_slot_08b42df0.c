/* Sets slot id and two bytes in a 0x2E8-stride record, with -1 meaning default. */
void func_08B42DF0(char *base, int idx, int a, int b)
{
    char *r = base + idx * 0x2E8 + 0xC;
    short four = 4;
    r[0x10] = -1;
    r[0x11] = 1;
    r[0x12] = -1;
    r[0x13] = 1;
    if (b == -1) {
        *(short *)r = four;
        r[0x10] = a;
    } else {
        *(short *)r = four;
        r[0x10] = a;
        r[0x11] = b;
    }
}
