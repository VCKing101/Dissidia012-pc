// Destructor: set the vtable at +4 and free the sized object when asked.
extern "C" char D_089C55F8[];
extern "C" void func_0898D4AC(void *self, int size);
struct Obj { int pad; void *vt; };
extern "C" void func_089C4E50(Obj *self, unsigned flags)
{
    if (self != 0) {
        self->vt = D_089C55F8;
        if (flags & 1) {
            func_0898D4AC(self, 8);
        }
    }
}
