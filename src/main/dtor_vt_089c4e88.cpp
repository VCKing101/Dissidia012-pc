// Destructor: set the vtable and free the object when asked.
extern "C" char D_089C5630[];
extern "C" void func_08992F04(void *self);
struct Obj { void *vt; };
extern "C" void func_089C4E88(Obj *self, unsigned flags)
{
    if (self != 0 && ((self->vt = D_089C5630), (flags & 1))) {
        func_08992F04(self);
    }
}
