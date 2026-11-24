/* Calls virtual 1 of the second member with the third argument, then forwards to the first member's handler. */
struct Sink { char data[4]; virtual void v0(); virtual void target(int n); };
struct Pair { int first; Sink *second; };
extern "C" void func_0898A448(int first, int n);
extern "C" void func_08983840(int a, int b, int c, Pair *p)
{
    p->second->target(c);
    func_0898A448(p->first, c);
}
