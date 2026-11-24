/* Calls virtual 2 of the sink at +4 (vptr in the sink at +4) unless bit 0 of the global flag is set, then runs the common step. */
struct Sink { char data[4]; virtual void v0(); virtual void v1(); virtual void target(int a); };
struct Owner { char pad[4]; Sink *sink; };
extern unsigned *D_09B6F478;
extern "C" void func_0898A5D8(Owner *self, int a);
extern "C" void func_0898A25C(Owner *self, int a)
{
    if ((*D_09B6F478 & 1) == 0) {
        self->sink->target(a);
    }
    func_0898A5D8(self, a);
}
