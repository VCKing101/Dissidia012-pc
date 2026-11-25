// Calls func_088D9CFC on each non-zero handle at self+4+4*i for slots 1..4 (slot 0 skipped).
extern "C" void func_088D9CFC(int h);
extern "C" void func_08B4F134(int *self)
{
    int i = 0;
    int *p = self;
    for (; i < 5; i++, p++) {
        if (i == 0) { continue; }
        if (p[1] != 0) {
            func_088D9CFC(p[1]);
        }
    }
}
