/* Sets up three arrays and three sub-objects, clears three members; returns self. */
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D2E70(void);
extern void func_088D31C4(void);
extern void func_08A74E80(void);
extern void func_08A75218(void *p);
extern void func_08A73C38(void *p);
extern void func_08A73CB8(void *p);
int func_08A75490(int self) {
    func_08993560((void *)self, 4, 4, (void *)func_088D2E70);
    func_08993560((void *)(self + 0x10), 3, 4, (void *)func_088D31C4);
    func_08993560((void *)(self + 0x1C), 0x10, 0x3C, (void *)func_08A74E80);
    func_08A75218((void *)(self + 0x3DC));
    func_08A73C38((void *)(self + 0x3F4));
    func_08A73CB8((void *)(self + 0x3F8));
    *(int *)(self + 0x420) = 0;
    *(int *)(self + 0x424) = 0;
    *(int *)(self + 0x428) = 0;
    return self;
}
