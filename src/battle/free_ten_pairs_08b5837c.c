/* Calls func_08808214 for each non-null pointer in ten 8-byte slots at +0xC and clears it. */
extern void func_08808214(void);
void func_08B5837C(char *self)
{
    int i = 0;
    int *p = (int *)(self + 0xC);
    do {
        int *q = p;
        if (*q != 0) {
            func_08808214();
            *q = 0;
        }
        i++;
        p += 2;
    } while (i < 10);
}
