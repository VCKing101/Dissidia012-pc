/* For the 3 handles at self+0x30, calls func_088D9B28(handle, flag) on each non-zero one. */
extern "C" void func_088D9B28(int h, unsigned char flag);
extern "C" void func_08B52DA8(int *self, unsigned char flag)
{
    for (int i = 0, *p = self; i < 3; i++, p++) {
        if (p[0x30 / 4] == 0) continue;
        func_088D9B28(p[0x30 / 4], flag);
    }
}
