/* Calls virtual 2 of the object that the fourth argument points at, with the third and fifth arguments. */
struct Target { virtual void v0(); virtual void v1(); virtual void target(int a, int b); };
extern "C" void func_08959A10(int a, int b, int c, Target **pp, int e)
{
    (*pp)->target(c, e);
}
