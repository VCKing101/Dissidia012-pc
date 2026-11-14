extern "C" {
extern char D_089CC710[];
extern void func_08992F04(void *p);
}

struct VtableAt24 {
    char pad_00[0x24];
    void *vtable;

    static void destroy(VtableAt24 *self, unsigned int flags, void *vt)
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
void func_088E66B4(VtableAt24 *self, unsigned int flags)
{
    VtableAt24::destroy(self, flags, D_089CC710);
}
}
