/* Forwards a request to virtual 3 of the sink at +0x1BC (vptr in the sink at +4) and records state 6; returns whether a sink exists. */
struct Sink { char data[4]; virtual void v0(); virtual void v1(); virtual void v2(); virtual void target(void *a, int b, int c); };
struct Owner { char pad[4]; char body[0x18C]; int state; char pad2[0x28]; Sink *sink; };
extern "C" bool func_0890AAE4(Owner *self, int a, int b)
{
    if (self->sink != 0) {
        self->sink->target(self->body, a, b);
        self->state = 6;
        return true;
    }
    return false;
}
