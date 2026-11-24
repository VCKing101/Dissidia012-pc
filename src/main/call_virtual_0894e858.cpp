// Calls func_08949940 on three arguments, forwards to virtual slot 2, returns 1.
struct V { virtual void v0(); virtual void v1(); virtual void v2(int a, int b, int c, float f); };
extern "C" void func_08949940(int a, int b, int c);
extern "C" int func_0894E858(V *o, int a, int b, int c, int d, float f)
{
    func_08949940(b, c, d);
    o->v2(a, b, c, f);
    return 1;
}
