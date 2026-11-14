extern "C" {
extern char D_089CCA28[];
extern void func_08992F04(void *p);
}

struct VtableAt0 {
    void *vtable;

    static void destroy(VtableAt0 *self, unsigned int flags, void *vt)
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
void func_088EC234(VtableAt0 *self, unsigned int flags)
{
    VtableAt0::destroy(self, flags, D_089CCA28);
}
}
