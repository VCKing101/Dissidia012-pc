int func_08ACD744(void *self, int value)
{
    if (value >= 0x24) {
        value = value + -0x18;
    } else {
        value = value % 0xC;
    }
    return value;
}
