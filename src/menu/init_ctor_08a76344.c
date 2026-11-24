/* Runs the base initialiser, sets up an array at +4 through the array helper; returns self. */
extern void func_088D2E70(void *p);
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D31C4(void);
void *func_08A76344(char *self) {
    func_088D2E70(self);
    func_08993560(self + 4, 2, 4, (void *)func_088D31C4);
    return self;
}
