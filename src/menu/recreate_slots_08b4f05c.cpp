/* After func_08B4F0D0(self), for slots 1..4 creates func_088DAA68(src) into self+4 and registers it with func_088DAB7C(src, h). */
extern "C" void func_08B4F0D0(int *self);
extern "C" int func_088DAA68(int src);
extern "C" void func_088DAB7C(int src, int h);
extern "C" void func_08B4F05C(int *self, int src)
{
    func_08B4F0D0(self);
    int i = 0;
    int *p = self;
    for (; i < 5; i++, p++) {
        if (i == 0) { continue; }
        {
            int h = func_088DAA68(src);
            p[1] = h;
            func_088DAB7C(src, h);
        }
    }
}
