// Destructor: set the vtable and free the object when asked.
extern "C" char D_089C5508[];
extern "C" void func_08992F04(void *self);
struct Obj { void *vt; };
extern "C" void func_089C2798(Obj *self, unsigned flags)
{
    if (self != 0 && ((self->vt = D_089C5508), (flags & 1))) {
        func_08992F04(self);
    }
}
