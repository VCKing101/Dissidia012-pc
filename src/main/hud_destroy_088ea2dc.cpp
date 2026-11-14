extern "C" {
extern char D_089CC9A8[];
extern void func_08992F04(void *p);
}

struct VtableAt54 {
    char pad_00[0x54];
    void *vtable;

    static void destroy(VtableAt54 *self, unsigned int flags, void *vt)
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
void func_088EA2DC(VtableAt54 *self, unsigned int flags)
{
    VtableAt54::destroy(self, flags, D_089CC9A8);
}
}
