/* Forwards three values to func_0892DC48 together with a handle from the object returned by func_08916FA0 (or 0). */
extern int func_08916FA0(void);
extern void func_0892DC48(float f, void *p, int a, int b, int c, int d, int e, int g);
void func_08B49FC8(int *self, int a, int b, int c) {
    int h = 0;
    int o = func_08916FA0();
    if (o != 0) {
        h = *(int *)(o + 0x5A0);
    }
    func_0892DC48(120.0f, self + 0x1A6, *self, 3, a, b, c, h);
}
