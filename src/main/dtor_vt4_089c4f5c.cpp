// Destructor: set the vtable at +4 and free the sized object when asked.
extern "C" char D_089C5650[];
extern "C" void func_0898D4AC(void *self, int size);
struct Obj { void *vt; };
extern "C" void func_089C4F5C(Obj *self, unsigned flags)
{
    if (self != 0) {
        self->vt = D_089C5650;
        if (flags & 1) {
            func_0898D4AC(self, 4);
        }
    }
}
