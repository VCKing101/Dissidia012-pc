/* Sets up three arrays and two sub-objects, clears two members; returns self. */
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D2E70(void);
extern void func_088D31C4(void);
extern void func_08A73070(void);
extern void func_08A73C38(void *p);
extern void func_08A73CB8(void *p);
int func_08A734B0(int self) {
    func_08993560((void *)self, 5, 4, (void *)func_088D2E70);
    func_08993560((void *)(self + 0x14), 6, 4, (void *)func_088D31C4);
    func_08993560((void *)(self + 0x2C), 10, 0x18, (void *)func_08A73070);
    func_08A73C38((void *)(self + 0x11C));
    func_08A73CB8((void *)(self + 0x120));
    *(int *)(self + 0x148) = 0;
    *(int *)(self + 0x14C) = 0;
    return self;
}
