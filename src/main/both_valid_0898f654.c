/* Returns whether both signed halfwords at 0xAA and 0xAC are non-negative. */
int func_0898F654(char *o) {
    short *p = (short *)(o + 0xAA);
    if (p[0] >= 0) {
        if (p[1] >= 0) { return 1; }
    }
    return 0;
}
