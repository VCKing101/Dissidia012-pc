/* Calls virtual 1 of the object that the fourth argument points at, with the third argument. */
struct Target { virtual void v0(); virtual void target(int a); };
extern "C" void func_08959A48(int a, int b, int c, Target **pp)
{
    (*pp)->target(c);
}
