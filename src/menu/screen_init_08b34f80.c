/* Builds the screen: sets up the widgets with the shared id, then runs its four layout passes. */
extern void func_08B3B408(void);
extern int func_08B42B40(void);
extern void func_0891F688(void *a, int b, int c);
extern void func_08B43680(int a, int b);
extern void func_08B4229C(int a, int b);
extern void func_08B427BC(int a, int b);
extern void func_08B3C7D8(void *self);
extern void func_08B3C2D4(void *self);
extern void func_08B3BFE4(void *self, int row);
extern void func_08B3FF0C(void *self);
extern void *D_089D8520;
extern int D_08B438C0;
extern int D_08B426B0;
extern int D_08B42A98;
extern short D_09B6DAC0;

void func_08B34F80(void *self)
{
    int id;
    func_08B3B408();
    id = func_08B42B40();
    func_0891F688(D_089D8520, 0, id);
    id = func_08B42B40();
    func_08B43680(D_08B438C0, id);
    id = func_08B42B40();
    func_08B4229C(D_08B426B0, id);
    id = func_08B42B40();
    func_08B427BC(D_08B42A98, id);
    func_08B3C7D8(self);
    func_08B3C2D4(self);
    func_08B3BFE4(self, D_09B6DAC0);
    func_08B3FF0C(self);
}
