/* memset (the game's C library). */
void *func_08935828(void *dst, int c, unsigned int n)
{
    char *p = (char *)dst;

    while (n-- != 0) {
        *p++ = c;
    }
    return dst;
}
