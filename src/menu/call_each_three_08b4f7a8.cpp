/* Calls func_088D9CFC for each of the three pointers at self+0x30 that are non-null. */
extern "C" void func_088D9CFC(void);
extern "C" void func_08B4F7A8(int *self)
{
    for (int i = 0, *p = self; i < 3; i++, p++) {
        if (p[0x30 / 4] != 0) { func_088D9CFC(); }
    }
}
