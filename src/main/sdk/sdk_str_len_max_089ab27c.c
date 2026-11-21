/* String library (built with -O3): length of s, at most max bytes (strnlen). */
unsigned int func_089AB27C(const char *s, unsigned int max)
{
    unsigned int n = 0;

    if (max != 0) {
        while (*s != 0) {
            n++;
            if (n >= max)
                break;
            s++;
        }
    }
    return n;
}
