// Release the two optional effect buffers of the linked object, then the next stage.
extern "C" void func_0897404C(void *p);
extern "C" void func_0894D730(int v);
struct Eff {
    char pad0[0x30];
    char first[0x610];
    char second[0x610];
    int h0;
    int h1;
};
struct Own { char pad0[8]; Eff *eff; char pad1[0x10]; int next; };
struct Rel {
    static void run(Own *self)
    {
        Eff *e = self->eff;
        if (e) {
            if (e->h0) {
                func_0897404C(e->first);
            }
            if (e->h1) {
                func_0897404C(e->second);
            }
        }
        func_0894D730(self->next);
    }
};
extern "C" void func_08940930(Own *self)
{
    Rel::run(self);
}
