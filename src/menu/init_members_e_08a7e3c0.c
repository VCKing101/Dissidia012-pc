/* Stores the vtable, initialises two sub-objects, two arrays and a third sub-object, stores two more pointers and a zero; returns self. */
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D2E70(void);
extern void func_088D31C4(void);
extern void func_08A74260(void *p);
extern void func_08A72D18(void *p);
extern void func_08A73C38(void *p);
extern char D_08A7E9C0[];
extern char D_08A7E9A8[];
int *func_08A7E3C0(int *self) {
    *self = (int)D_08A7E9C0;
    func_08A74260(self + 1);
    func_08A72D18(self + 5);
    func_08993560(self + 0xC, 5, 4, (void *)func_088D2E70);
    func_08993560(self + 0x11, 1, 4, (void *)func_088D31C4);
    func_08A73C38(self + 0x12);
    self[0x18] = (int)D_08A7E9A8;
    self[0x19] = (int)self;
    self[0x1A] = 0;
    return self;
}
