// Releases resources held in three arrays of an object (2 effects at +0x20, 3 models at +0x14, 5 handles at +0), clearing each slot.
extern "C" void func_08B16EB0(int *self);
extern "C" {
extern void func_0887BA5C(int p);
extern void func_088DA060(int p);
extern void func_088DAB44(int p);
}
extern "C" void func_08B16EB0(int *self)
{
    for (int i = 0, *p = self; i < 2; i++, p++) {
        if (p[8] != 0) {
            func_0887BA5C(p[8]);
            p[8] = 0;
        }
    }
    for (int i = 0, *p = self; i < 3; i++, p++) {
        if (p[5] != 0) {
            func_088DA060(p[5]);
            p[5] = 0;
        }
    }
    for (int i = 0, *p = self; i < 5; i++, p++) {
        if (p[0] != 0) {
            func_088DAB44(p[0]);
            p[0] = 0;
        }
    }
}
