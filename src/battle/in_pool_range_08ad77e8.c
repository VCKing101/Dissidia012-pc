/* Returns 1 when the address lies inside the 0x2800-byte pool at D_089D4930. */
extern unsigned D_089D4930;

unsigned char func_08AD77E8(unsigned p)
{
    int r = 0;
    if (p >= D_089D4930 && p < D_089D4930 + 0x2800) {
        r = 1;
    }
    return r;
}
