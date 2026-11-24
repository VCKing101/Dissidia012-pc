/* In state 1, forwards the arguments to virtual 8 of the sink at +0x1BC and virtual 2 of the object at +0x1B4. */
struct SinkA { char data[4]; virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void target(int a, int b, int c); };
struct SinkB { virtual void v0(); virtual void v1(); virtual void target(int a); };
struct Owner { char pad[0x190]; int state; char pad2[0x20]; SinkB *b; char pad3[4]; SinkA *a; };
extern "C" void func_0890AF9C(Owner *self, int a, int b, int c)
{
    if (self->state == 1) {
        if (self->a != 0) {
            self->a->target(a, b, c);
        }
        if (self->b != 0) {
            self->b->target(a);
        }
    }
}
