/* String library (built with -O3; also matches at -O2): byte fill (memset). */
void func_089AB36C(unsigned char *p, int value, unsigned int n)
{
    while (n != 0) {
        n--;
        *p++ = value;
    }
}
