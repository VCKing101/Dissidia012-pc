/* String library (built with -O3): 1 if the first n bytes of a and b differ, else 0. */
int func_089AB30C(const char *a, const char *b, unsigned int n)
{
    for (; n != 0; n--, a++, b++)
        if (*a != *b)
            return 1;
    return 0;
}
