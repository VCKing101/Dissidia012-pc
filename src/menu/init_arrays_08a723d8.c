/* Sets up two arrays (7 and 3 elements of size 4) through the array helper; returns self. */
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D2E70(void);
extern void func_088D31C4(void);
int func_08A723D8(int self) {
    func_08993560((void *)self, 7, 4, (void *)func_088D2E70);
    func_08993560((void *)(self + 0x1C), 3, 4, (void *)func_088D31C4);
    return self;
}
