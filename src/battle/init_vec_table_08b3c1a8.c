/* Sets eight 3-float entries at +0x6F0 to (1, 1, 0). */
void func_08B3C1A8(char *self) {
    int i = 0;
    float *p = (float *)(self + 0x6F0);
    do {
        p[0] = 1.0f;
        p[1] = 1.0f;
        p[2] = 0.0f;
        i++;
        p += 3;
    } while (i < 8);
}
