/* Starts a 3-second-style fade scaled by func_088D4C5C, with func_08B09440 as the callback, then updates the sub-object and mixer. */
extern void *D_089D6A10;
extern int func_088D4C5C(void);
extern void func_08873DEC(int a, int b);
extern void func_08873D34(int mode, void (*callback)(void *), void *owner, int d, int e, float time);
extern void func_08B09440(void *owner);
extern void func_08B098A4(void *sub);
extern void func_08889060(void *p);

void func_08B07F54(char *self)
{
    int ticks = func_088D4C5C();
    func_08873DEC(3, 2);
    func_08873D34(3, func_08B09440, self, 0, 2, (float)ticks);
    func_08B098A4(self + 0xA4);
    func_08889060(D_089D6A10);
}
