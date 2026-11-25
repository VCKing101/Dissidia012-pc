// Destroys an object when set: runs two teardown calls, resets the sub-object vtable at +0x180, and frees it when bit 0 of flags is set.
extern "C" {
extern void func_0880FC58(void *p);
extern void func_0880FBA0(void *p, int n);
extern void func_08B3B934(void *p);
extern char D_08B3F1A8[];
}
struct Sub { char *vt; };
struct Obj {
    char data[0x180];
    Sub sub;
    char more[0x5C];
    static void run(Obj *self, unsigned flags)
    {
        if (self != 0) {
            func_0880FC58((char *)self + 0x1E0);
            func_0880FBA0((char *)self + 0x1E0, 2);
            if (&self->sub != 0) {
                self->sub.vt = D_08B3F1A8;
            }
            if (flags & 1) {
                func_08B3B934(self);
            }
        }
    }
};
extern "C" void func_08B3AD98(Obj *self, unsigned flags) { Obj::run(self, flags); }
