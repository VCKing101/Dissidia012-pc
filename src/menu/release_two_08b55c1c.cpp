/* Releases twelve handles from +8 with func_088DA060 and two handles at +0 with func_088DAB44, clearing each. */
extern "C" void func_088DA060(int h);
extern "C" void func_088DAB44(int h);
extern "C" void func_08B55C1C(int *self)
{
    for (int i = 0, *p = self; i < 12; i++, p++) {
        if (p[8 / 4] != 0) {
            func_088DA060(p[8 / 4]);
            p[8 / 4] = 0;
        }
    }
    for (int i = 0, *p = self; i < 2; i++, p++) {
        if (p[0] != 0) {
            func_088DAB44(p[0]);
            p[0] = 0;
        }
    }
}
