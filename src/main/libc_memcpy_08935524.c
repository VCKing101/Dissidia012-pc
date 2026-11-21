/* memcpy (the game's C library). The image library copies GIM blocks out of package files with it. */
void *func_08935524(void *dst, const void *src, unsigned int n)
{
    char *d = (char *)dst;
    void *ret = dst;
    const char *s = (const char *)src;

    while (n-- != 0) {
        *d++ = *s++;
    }
    return ret;
}
