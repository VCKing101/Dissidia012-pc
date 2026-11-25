extern "C" void func_08904978(void);
extern "C" void func_0885C538(int, int, int);
extern "C" void func_08905848(int *);
extern "C" int func_089C57B0(void);
extern "C" int func_089C5788(void);
extern "C" bool func_0890380C(int *self);
extern "C" bool func_0890380C(int *self)
{
    func_08904978();
    if (self[0x43] != 0) {
        int i = 0;
        int *q = self;
        int *p = q + 2;
        do {
            if (q[4] != 0) {
                func_0885C538(self[0x43], q[4], 0);
            }
            p[2] = 0;
            p[3] = 0;
            func_08905848(p);
            i = i + 1;
            q = q + 4;
            p = p + 4;
        } while (i < 0x10);
        self[0x43] = 0;
    }
    if (*self != -1) {
        int r = func_089C57B0();
        if (r >= 0) {
            *self = -1;
        } else {
            return false;
        }
    }
    int r2 = func_089C5788();
    if (r2 < 0) {
        return false;
    }
    return true;
}
