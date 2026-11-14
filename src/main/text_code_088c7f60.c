/* Packs a text code: high byte, 0xE000 marker, bits 5+ and the low bits. */
unsigned int func_088C7F60(unsigned int a, unsigned int b, unsigned int c)
{
    unsigned int v = (a << 8) | 0xE000;
    v |= b << 5;
    return v | c;
}
