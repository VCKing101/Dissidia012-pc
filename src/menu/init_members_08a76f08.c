/* Sets up an array, then initialises three members; returns self. */
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D2E70(void);
extern void func_088D31C4(void *p);
extern void func_08A76C9C(void *p);
extern void func_08A73C38(void *p);
int func_08A76F08(int self) {
    func_08993560((void *)self, 3, 4, (void *)func_088D2E70);
    func_088D31C4((void *)(self + 0xC));
    func_08A76C9C((void *)(self + 0x10));
    func_08A73C38((void *)(self + 0x78));
    return self;
}
