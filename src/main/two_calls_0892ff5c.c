/* Calls func_08930158(self, a1, a2), then func_08930224(self, a1, a2, 0x17); sets the word at 0x28 to 1. */
extern void func_08930158(int *, int, int);
extern void func_08930224(int *, int, int, int);
void func_0892FF5C(int *self, int a1, int a2) {
    int c = a2;
    int *s = self;
    int b = a1;
    func_08930158(s, b, c);
    func_08930224(s, b, c, 0x17);
    s[0x28 / 4] = 1;
}
