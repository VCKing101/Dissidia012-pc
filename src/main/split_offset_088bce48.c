/* Splits the negated value into its low byte and its high byte minus 0x20. */
void func_088BCE48(short value, unsigned int *low, unsigned int *high)
{
    short v = -value;
    *low = v & 0xFF;
    *high = (v >> 8) & 0xFF;
    *high -= 0x20;
}
