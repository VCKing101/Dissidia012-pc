/* Swaps the active buffer index (word 0xC, 0 or 1) of a double-buffered object and stores the size difference at 0x14. */
void func_08974A00(int *p) {
    int i = p[3]; int cur = p[2]; int one = 1; int d = cur - p[i]; i = one - i; p[5] = d; int *q = &p[i]; p[3] = i; p[2] = *q;
}
