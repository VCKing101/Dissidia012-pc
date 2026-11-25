// Calls func_08B59204 on each of ten 0xD0-byte entries starting at +0x10.
extern "C" void func_08B59204(void *);
extern "C" void func_08B58AC8(char *self)
{
    for (int i = 0, *p = (int *)(self + 0x10); i < 10; i++, p += 0x34) {
        func_08B59204(p);
    }
}
