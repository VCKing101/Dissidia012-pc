/* Returns whether bits 27..30 of the argument equal 14. */
int func_089471A8(int a) {
    int t = (a >> 27) & 0xF;
    if (t == 14) { return 1; }
    return 0;
}
