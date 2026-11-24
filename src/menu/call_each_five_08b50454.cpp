/* Calls func_088D9CFC for each of the five pointers at self+4 that are non-null. */
extern "C" void func_088D9CFC(void);
extern "C" void func_08B50454(int *self)
{
    for (int i = 0, *p = self; i < 5; i++, p++) {
        if (p[1] != 0) { func_088D9CFC(); }
    }
}
