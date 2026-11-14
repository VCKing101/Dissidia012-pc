extern "C" {
extern char D_089CC7A0[];
extern void func_08992F04(void *p);
}

struct VtableAt4C {
    char pad_00[0x4C];
    void *vtable;

    static void destroy(VtableAt4C *self, unsigned int flags, void *vt)
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
void func_088E81FC(VtableAt4C *self, unsigned int flags)
{
    VtableAt4C::destroy(self, flags, D_089CC7A0);
}
}
