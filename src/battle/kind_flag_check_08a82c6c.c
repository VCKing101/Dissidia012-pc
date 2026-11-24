/* Returns 1 when the entry's kind byte is one of a fixed set (4,5,7,0x10,0x16,0x26). */
int func_08A82C6C(char *p, int n)
{
    int r;
    if (p == 0 || n <= 0) {
        return 0;
    }
    r = 0;
    switch (*p) {
    case 4:
    case 5:
    case 7:
    case 0x10:
    case 0x16:
    case 0x26:
        r = 1;
    }
    return r;
}
