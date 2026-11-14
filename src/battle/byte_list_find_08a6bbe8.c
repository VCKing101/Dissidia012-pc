/* True when the byte list holds the value 0x13. */
int func_08A6BBE8(void *owner, signed char *list, int count)
{
    int result;

    if (list == 0 || count < 0) {
        return 0;
    }
    {
        int i = 0;
        result = 0;
        while (i < count) {
            if (*((signed char *)((int)list + i)) != 0x13) {
                i++;
                continue;
            }
            return 1;
        }
    }
    return result;
}
