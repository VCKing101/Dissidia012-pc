/* Sets up three arrays through the array helper and clears the members at +0x74 and +0x78; returns self. */
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D2E70(void);
extern void func_088D31C4(void);
extern void func_08A75B78(void);
int func_08A75D14(int self) {
    func_08993560((void *)self, 2, 4, (void *)func_088D2E70);
    func_08993560((void *)(self + 8), 2, 4, (void *)func_088D31C4);
    func_08993560((void *)(self + 0x10), 5, 0x14, (void *)func_08A75B78);
    *(int *)(self + 0x74) = 0;
    *(int *)(self + 0x78) = 0;
    return self;
}
