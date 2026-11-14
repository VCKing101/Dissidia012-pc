/* Text code with bit 15 set, plus bit 13 for the alternate set. */
unsigned int func_088C7F3C(int alt, unsigned int code)
{
    unsigned int v = code | 0x8000;
    if (alt == 0) {
        return v;
    }
    return v | 0x2000;
}
