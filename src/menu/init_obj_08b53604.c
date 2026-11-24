/* Constructor-like init: base init, clears +0x28/+0x2C, two handles at +0x30 and 1 at +0x38. */
extern void func_0892AC2C(int *self);
void func_08B53604(int *self)
{
    int i;
    int *p;
    func_0892AC2C(self);
    self[0x28 / 4] = 0;
    self[0x2C / 4] = 0;
    for (i = 0, p = self; i < 2; i++, p++) {
        p[0x30 / 4] = 0;
    }
    for (i = 0, p = self; i < 1; i++, p++) {
        p[0x38 / 4] = 0;
    }
}
