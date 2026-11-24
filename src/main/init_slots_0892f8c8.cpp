// Initialises two header fields, then runs the slot initialiser over the 0x10C-byte slot array.
extern "C" void func_088D3F00(int, void *, const void *);
extern "C" void func_088D44EC(void *, void *, const void *);
extern "C" void func_0892ED30(void *);
extern char D_089CF080[], D_089CF090[];
extern "C" void func_0892F8C8(char *self) {
    char *q = self + 0x34;
    func_088D3F00(0, q, D_089CF080);
    func_088D44EC(q, self + 0x38, D_089CF090);
    for (int i = 0, *p = (int *)(self + 0x3C); i < 1; i++, p = (int *)((char *)p + 0x10C)) func_0892ED30(p);
}
