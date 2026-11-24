/* Sets up two arrays (5 and 4 elements of size 4) through the array helper and clears the member at +0x24; returns self. */
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D2E70(void);
extern void func_088D31C4(void);
int func_08A73CB8(int self) {
    func_08993560((void *)self, 5, 4, (void *)func_088D2E70);
    func_08993560((void *)(self + 0x14), 4, 4, (void *)func_088D31C4);
    *(int *)(self + 0x24) = 0;
    return self;
}
