/* Stores a random 15-bit value and the given second word into the pair. */
extern int D_09AF84D0;

void func_08ACD9B4(unsigned *out, unsigned value)
{
    out[0] = (D_09AF84D0 = D_09AF84D0 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    out[1] = value;
}
