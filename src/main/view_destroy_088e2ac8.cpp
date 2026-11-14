/* Deleting destructor: restores the base vtable at 0x7BC. */
extern "C" {
extern char D_089CC538[];
extern void func_08992F04(void *p);
}

struct VtableAt7BC {
    char pad_000[0x7BC];
    void *vtable;

    static void destroy(VtableAt7BC *self, unsigned int flags, void *vt)
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
void func_088E2AC8(VtableAt7BC *self, unsigned int flags)
{
    VtableAt7BC::destroy(self, flags, D_089CC538);
}
}
