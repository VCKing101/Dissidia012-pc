/* Packs five fields into one word: bits 20+, 16+, 11+, 6+ and the low bits. */
unsigned int func_088ADFA8(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e)
{
    unsigned int v = a << 20;
    v |= b << 16;
    v |= c << 11;
    v |= d << 6;
    return v | e;
}
