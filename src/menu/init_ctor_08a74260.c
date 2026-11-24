/* Sets up a three-by-four array through the array helper, then initialises the member at +0xC; returns self. */
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D31C4(void *p);
extern void func_088D2E70(void);
void *func_08A74260(char *self) {
    func_08993560(self, 3, 4, (void *)func_088D2E70);
    func_088D31C4(self + 0xC);
    return self;
}
