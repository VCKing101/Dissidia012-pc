/* String library (built with -O3): byte copy (memcpy). */
void func_089AB344(unsigned char *d, const unsigned char *s, unsigned int n)
{
    while (n != 0) {
        n--;
        *d++ = *s++;
    }
}
