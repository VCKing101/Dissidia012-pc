/* For the two handles at self+0x30 calls func_088D9B28(handle, flag). */
extern "C" void func_088D9B28(int h, int flag);
extern "C" void func_08B513AC(int *self, unsigned char flag)
{
    for (int i = 0, *p = self; i < 2; i++, p++) {
        func_088D9B28(p[0x30 / 4], flag);
    }
}
