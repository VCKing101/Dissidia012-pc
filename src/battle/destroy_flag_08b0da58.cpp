// If self is set: func_08B0DB40(self), func_0887A644 on the sub-object at +8 (member +0x4C) when that is non-null, and func_08992F04(self) when bit 0 of flags is set.
extern "C" {
extern void func_08B0DB40(void *);
extern void func_0887A644(void *, int);
extern void func_08992F04(void *);
}
struct Sub08B0DA58 { char pad[0x4C]; char m[1]; };
struct Obj08B0DA58 {
    char pad[8];
    Sub08B0DA58 sub;
    static void run(Obj08B0DA58 *self, unsigned flags)
    {
        if (self != 0) {
            func_08B0DB40(self);
            if (&self->sub != 0) {
                func_0887A644(self->sub.m, 2);
            }
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};
extern "C" void func_08B0DA58(Obj08B0DA58 *self, unsigned flags)
{
    Obj08B0DA58::run(self, flags);
}
