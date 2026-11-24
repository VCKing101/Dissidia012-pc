/* Calls the 0x08865158 hook on self + 0x100 when 0x0887E718 says the object at 0x089D6998 is idle. */
extern "C" void *D_089D6998;
extern "C" int func_0887E718(void *p);
extern "C" void func_08865158(char *p);
struct Kept { char *v; Kept(char *s) { v = s; } };
extern "C" void func_08B320F0(char *self)
{
    Kept k(self);
    int r = func_0887E718(D_089D6998);
    self = k.v;
    if (r) { return; }
    func_08865158(self + 0x100);
}
