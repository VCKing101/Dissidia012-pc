/* String library (built with -O3): append src to the string in a buffer of size bytes. */
static unsigned int str_len_max(const char *s, unsigned int max)
{
    unsigned int n = 0;

    while (n < max && *s != 0) {
        n++;
        s++;
    }
    return n;
}

void func_089AB210(char *dst, const char *src, unsigned int size);

void func_089AB2B4(char *dst, const char *src, unsigned int size)
{
    unsigned int len = str_len_max(dst, size);

    func_089AB210(dst + len, src, size - len);
}
