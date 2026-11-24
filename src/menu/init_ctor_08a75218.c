/* Sets up a three-by-four array through the array helper, then clears three members; returns self. */
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D2E70(void);
int func_08A75218(int self) {
    func_08993560((void *)self, 3, 4, (void *)func_088D2E70);
    *(int *)(self + 0xC) = 0;
    *(int *)(self + 0x10) = 0;
    *(int *)(self + 0x14) = 0;
    return self;
}
