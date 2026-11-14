extern "C" {
extern char D_089CC9C8[];
extern void func_08992F04(void *p);
}

struct VtableAt20 {
    char pad_00[0x20];
    void *vtable;

    static void destroy(VtableAt20 *self, unsigned int flags, void *vt)
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
void func_088EA760(VtableAt20 *self, unsigned int flags)
{
    VtableAt20::destroy(self, flags, D_089CC9C8);
}
}
