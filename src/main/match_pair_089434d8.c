/* Returns whether the shorts at 4 and 6 of the second argument equal the words D_089D08A8 and D_089D08AC. */
extern int D_089D08A8;
extern int D_089D08AC;
int func_089434D8(int a0, short *p) {
    if (p[2] != D_089D08A8) { return 0; }
    if (p[3] != D_089D08AC) { return 0; }
    return 1;
}
