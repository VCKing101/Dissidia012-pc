/* Deleting destructor: restores the base vtable (at 0x24), frees when bit 0 of flags is set. */
struct DtorObject;

extern "C" {
void func_08992F04(DtorObject *self);
extern int D_089C8E28;
}

struct DtorObject {
    char pad_00[0x24];
    void *vtable;

    static void destroy(DtorObject *self, unsigned int flags, void *vt)
    {
        if (self != 0) {
            self->vtable = vt;
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {

void func_0885C4D4(DtorObject *self, unsigned int flags)
{
    DtorObject::destroy(self, flags, &D_089C8E28);
}

}
