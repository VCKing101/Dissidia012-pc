/* Return the index of the entry whose two halfwords equal the key pair, or -1. */
int func_08926550(int unused, int base, short a, short b)
{
    short *p = (short *)(*(int *)(base + 0x480) + base);
    int i = 0;
    int res = -1;
    if (i < *(unsigned char *)(base + 2)) {
        do {
            if (p[0] == a && p[1] == b) {
                res = i;
                break;
            }
            i++;
            p += 2;
        } while (i < *(unsigned char *)(base + 2));
    }
    return res;
}
