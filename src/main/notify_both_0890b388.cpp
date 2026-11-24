/* Forwards the three arguments to virtual 16 of the object at +0x1BC and virtual 10 of the object at +0x1B4, when present. */
struct SinkA {
    char data[4];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void target(int a, int b, int c);
};
struct SinkB {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9();
    virtual void target(int a, int b, int c);
};
struct Owner { char pad[0x1B4]; SinkB *b; char pad2[4]; SinkA *a; };
extern "C" void func_0890B388(Owner *self, int a, int b, int c)
{
    if (self->a) {
        self->a->target(a, b, c);
    }
    if (self->b) {
        self->b->target(a, b, c);
    }
}
