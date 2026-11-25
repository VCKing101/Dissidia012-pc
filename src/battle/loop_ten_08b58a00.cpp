// Calls func_08B58D68 on each of ten 0xD0-byte entries starting at +0x10.
extern "C" void func_08B58D68(void *);
extern "C" void func_08B58A00(char *self)
{
    for (int i = 0, *p = (int *)(self + 0x10); i < 10; i++, p += 0x34) {
        func_08B58D68(p);
    }
}
