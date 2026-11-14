extern "C" {
extern char D_089CC738[];
extern void func_08992F04(void *p);
}

struct VtableAt418 {
    char pad_000[0x418];
    void *vtable;

    static void destroy(VtableAt418 *self, unsigned int flags, void *vt)
    {
        if (self != 0) {
            self->vtable = vt;
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
void func_088E6E18(VtableAt418 *self, unsigned int flags)
{
    VtableAt418::destroy(self, flags, D_089CC738);
}
}
