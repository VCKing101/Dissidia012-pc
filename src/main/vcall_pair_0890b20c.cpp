/* Asks the sink at +0x1BC (virtual 13) and, when it answers 1, records state 7; then notifies the object at +0x1B4 (virtual 7). */
struct SinkA { char data[4]; virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12(); virtual int target(int a); };
struct SinkB { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void target(int a); };
struct Owner { char pad[0x190]; int state; char pad2[0x20]; SinkB *b; char pad3[4]; SinkA *a; };
extern "C" void func_0890B20C(Owner *self, int arg)
{
    if (self->a != 0) {
        if (self->a->target(arg) == 1) {
            self->state = 7;
        }
    }
    if (self->b != 0) {
        self->b->target(arg);
    }
}
