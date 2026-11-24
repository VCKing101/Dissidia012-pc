/* If the handle at +0 is set, releases it with func_0899BB1C, clears it and calls func_08B45288. */
extern void func_0899BB1C(int h);
extern void func_08B45288(int *self);
void func_08B45340(int *self)
{
    if (self[0] != 0) {
        func_0899BB1C(self[0]);
        self[0] = 0;
        func_08B45288(self);
    }
}
