/* Releases five handles at +0 with func_088DA060 and three handles at +0x14 with func_0887BA5C, clearing each. */
extern "C" void func_088DA060(int h);
extern "C" void func_0887BA5C(int h);
extern "C" void func_08B554EC(int *self)
{
    for (int i = 0, *p = self; i < 5; i++, p++) {
        if (p[0] != 0) {
            func_088DA060(p[0]);
            p[0] = 0;
        }
    }
    for (int i = 0, *p = self; i < 3; i++, p++) {
        if (p[0x14 / 4] != 0) {
            func_0887BA5C(p[0x14 / 4]);
            p[0x14 / 4] = 0;
        }
    }
}
