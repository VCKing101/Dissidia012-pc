/* If the handle at *p is valid (not -1 and func_089C5778 non-negative), calls func_08904978(p) and returns 1; else 0. */
extern "C" int func_089C5778(int h);
extern "C" void func_08904978(int *p);
extern "C" int func_08903C58(int *p)
{
    int *keep[1];
    int cur = *p;
    int invalid = -1;
    if (cur != invalid) {
        keep[0] = p;
        int r = func_089C5778(*p);
        p = keep[0];
        if (r < 0) {
            return 0;
        } else {
            func_08904978(p);
            return 1;
        }
    } else {
        return 0;
    }
}
