int func_089FD4A4(void *self, int value, int total)
{
    int result = 0;
    if (value > 1) {
        int scaled = 0x16;
        if (value < total) {
            scaled = (short)(value * 0x16 / total);
        }
        result = scaled;
    }
    return result;
}
