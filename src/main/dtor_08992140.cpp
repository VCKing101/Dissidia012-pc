// Destructor: reset the vtable, release the resources and free the object when asked.
extern "C" char D_089DD380[];
extern "C" void func_089C5CC8(void);
extern "C" void func_08934B18(int h);
extern "C" void func_089928FC(void *self);
struct Obj {
    int pad0;
    int id;
    char pad1[0x2C];
    int handle;
    char pad2[0x18];
    void *vt;
};
extern "C" void func_08992140(Obj *self, unsigned flags)
{
    if (self != 0) {
        self->vt = D_089DD380;
        if (self->id >= 0) {
            func_089C5CC8();
        }
        func_08934B18(self->handle);
        if (flags & 1) {
            func_089928FC(self);
        }
    }
}
