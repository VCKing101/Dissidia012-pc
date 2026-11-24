/* Bump-allocates n bytes (rounded up to 16) from the current half of a double buffer; returns 0 when it does not fit. */
unsigned func_08974A3C(int *b, int n) {
    unsigned limit = b[b[3]] + b[4];
    unsigned cur = b[2];
    unsigned end = cur + ((n + 15) & -16);
    if (limit < end) {
        return 0;
    } else {
        b[2] = end;
    }
    return cur;
}
