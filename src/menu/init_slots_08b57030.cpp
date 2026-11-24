/* Clears +8 and +0x34, runs func_08935828(self, 0, 8), then zeroes five eight-byte slots from +0xC; returns self. */
extern "C" void func_08935828(int *self, int a, int b);
extern "C" int *func_08B57030(int *self)
{
    self[2] = 0;
    self[0x34 / 4] = 0;
    func_08935828(self, 0, 8);
    for (int i = 0, *p = self + 3; i < 5; i++, p += 2) {
        p[0] = 0;
        p[1] = 0;
    }
    return self;
}
