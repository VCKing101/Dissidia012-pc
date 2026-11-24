/* After func_08B560E4, calls func_088D9CD8 on each non-zero handle in the five words starting at +0x1C, then sets the state word at +0x50 to 4. */
extern "C" void func_08B560E4(int *self);
extern "C" void func_088D9CD8(int h);
extern "C" void func_08B55EE4(int *self)
{
    func_08B560E4(self);
    for (int i = 5, *p = self + 5; i < 10; i++, p++) {
        if (p[2] != 0) {
            func_088D9CD8(p[2]);
        }
    }
    self[0x50 / 4] = 4;
}
