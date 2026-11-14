extern "C" {
extern void func_08992F04(void *p);
extern char D_089CDD88[];

/* Bound-callback holder whose handler table sits at +4. */
struct CallbackHolder {
    int field_00;
    void *vtable;

    static void destroy(CallbackHolder *self, unsigned int flags, void *vt)
    {
        if (self != 0) {
            self->vtable = vt;
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

void func_0890B758(CallbackHolder *self, unsigned int flags)
{
    CallbackHolder::destroy(self, flags, D_089CDD88);
}
}
