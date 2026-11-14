/* Splits a code into its high part and its low byte. */
void func_088CA664(unsigned int code, unsigned int *high, unsigned int *low)
{
    *low = code;
    *low &= 0xFF;
    *high = code;
    *high >>= 8;
}
