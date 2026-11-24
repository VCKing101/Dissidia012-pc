/* Sets up two arrays (3 and 2 elements of size 4) through the array helper and clears the member at +0x14; returns self. */
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D2E70(void);
extern void func_088D31C4(void);
int func_08A73070(int self) {
    func_08993560((void *)self, 3, 4, (void *)func_088D2E70);
    func_08993560((void *)(self + 0xC), 2, 4, (void *)func_088D31C4);
    *(int *)(self + 0x14) = 0;
    return self;
}
