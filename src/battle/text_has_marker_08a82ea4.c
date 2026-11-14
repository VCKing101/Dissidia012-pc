/* True when the text holds one of the marker bytes 0x25, 0x22 or 0x12. */
int func_08A82EA4(signed char *text, int length)
{
    int result;

    if (text == 0 || length <= 0) {
        return 0;
    }
    {
        int i = 0;
        result = 0;
        while (i < length) {
            signed char c = *((signed char *)((int)text + i));
            if (c == 0x25 || c == 0x22 || c == 0x12) {
                result = 1;
            }
            if (result == 1) {
                break;
            }
            i++;
        }
    }
    return result;
}
