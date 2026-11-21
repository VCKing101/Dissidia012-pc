/* String library (built with -O3): copy src into a buffer of size bytes, always terminated
 * (the last byte is overwritten when src does not fit). */
static unsigned int str_len_max(const char *s, unsigned int max)
{
    unsigned int n = 0;

    while (n < max && *s != 0) {
        n++;
        s++;
    }
    return n;
}

void func_089AB210(char *dst, const char *src, unsigned int size)
{
    unsigned int len = str_len_max(src, size);
    unsigned int i;

    for (i = 0; i < len; i++)
        *dst++ = *src++;
    dst[-(i == size)] = 0;
}
