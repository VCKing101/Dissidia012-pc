/* Calls func_08931DA0(self), clears the one word at +0x10 and the three words at +0x14. */
extern void func_08931DA0(int *self);
void func_08B4CAD4(int *self)
{
    int i;
    int *p;
    func_08931DA0(self);
    i = 0;
    p = self;
    do { p[0x10 / 4] = 0; i++; p++; } while (i < 1);
    i = 0;
    p = self;
    do { p[0x14 / 4] = 0; i++; p++; } while (i < 3);
}
