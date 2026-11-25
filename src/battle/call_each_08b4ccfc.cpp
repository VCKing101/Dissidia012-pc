// Calls func_088D9CD8 with each non-null pointer of eight slots at +8.
extern "C" void func_088D9CD8(void *);
extern "C" void func_08B4CCFC(int *self)
{
    for (int i = 0, *p = self; i < 8; i++, p++) {
        if (p[2] != 0) {
            func_088D9CD8((void *)p[2]);
        }
    }
}
