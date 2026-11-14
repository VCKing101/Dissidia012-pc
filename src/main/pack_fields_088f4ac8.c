/* Packs five fields into one word (bits 20, 16, 11, 6, 0). */
unsigned int func_088F4AC8(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e)
{
    unsigned int packed = (a << 20) | (b << 16);

    packed |= c << 11;
    packed |= d << 6;
    return packed | e;
}
