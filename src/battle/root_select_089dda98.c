extern void func_089E8EE8(void *, unsigned int);

int func_089DDA98(unsigned char *object, unsigned int slot)
{
    if (!object[0]) return -1;
    if (slot >= 3) return -2;
    func_089E8EE8(object + 0x10, slot);
    return 0;
}
