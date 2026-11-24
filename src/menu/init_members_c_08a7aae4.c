/* Stores the vtable, initialises the base member and two arrays and a sub-object, stores two more pointers; returns self. */
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D2E70(void);
extern void func_088D31C4(void);
extern void func_08A76F08(void *p);
extern void func_08A73C38(void *p);
extern char D_08A7B2F8[];
extern char D_08A7B2E0[];
int *func_08A7AAE4(int *self) {
    *self = (int)D_08A7B2F8;
    func_08A76F08(self + 1);
    func_08993560(self + 0x20, 5, 4, (void *)func_088D2E70);
    func_08993560(self + 0x25, 1, 4, (void *)func_088D31C4);
    func_08A73C38(self + 0x26);
    self[0x2C] = (int)D_08A7B2E0;
    self[0x2D] = (int)self;
    return self;
}
