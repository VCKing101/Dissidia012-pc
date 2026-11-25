// Releases the two sprites and 0x23 effects of an object, clearing each slot, then resets its state fields.
extern "C" {
extern void func_088DA060(int p);
extern void func_0887BA5C(int p);
}
extern "C" void func_08B4B430(int *self)
{
    for (int i = 0, *p = self; i < 2; i++, p++) {
        if (*p != 0) { func_088DA060(*p); *p = 0; }
    }
    for (int i = 0, *p = self; i < 0x23; i++, p++) {
        if (p[5] != 0) { func_0887BA5C(p[5]); p[5] = 0; }
    }
    self[2] = 0; self[3] = 0; self[4] = 0; self[0x29] = -1;
    *((char *)self + 0x24D) = 0;
}
