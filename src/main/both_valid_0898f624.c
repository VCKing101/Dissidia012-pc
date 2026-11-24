/* Returns whether both signed halfwords at 0xA4 and 0xA6 are non-negative. */
int func_0898F624(char *o) {
    short *p = (short *)(o + 0xA4);
    if (p[0] >= 0) {
        if (p[1] >= 0) { return 1; }
    }
    return 0;
}
