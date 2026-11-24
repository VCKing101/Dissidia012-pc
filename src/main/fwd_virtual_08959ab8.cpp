/* Calls virtual 3 of the object that the fourth argument points at, with the third and fifth arguments; returns 1. */
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); virtual void target(int a, int b); };
extern "C" int func_08959AB8(int a, int b, int c, Target **pp, int e)
{
    (*pp)->target(c, e);
    return 1;
}
