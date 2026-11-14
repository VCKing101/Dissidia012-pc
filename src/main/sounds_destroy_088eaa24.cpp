extern "C" {
extern char D_089CC9E8[];
extern void func_08992F04(void *p);
}

struct VtableAt80 {
    char pad_00[0x80];
    void *vtable;

    static void destroy(VtableAt80 *self, unsigned int flags, void *vt)
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
void func_088EAA24(VtableAt80 *self, unsigned int flags)
{
    VtableAt80::destroy(self, flags, D_089CC9E8);
}
}
