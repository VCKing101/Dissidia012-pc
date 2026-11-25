// Destructor of a derived class: set the base then the own vtable, free when asked.
extern "C" char D_089C5688[];
extern "C" char D_089C5650[];
extern "C" void func_0898D4AC(void *self, int size);
struct Obj { void *vt; };
extern "C" void func_089C4FBC(Obj *self, unsigned flags)
{
    if (self != 0 && ((self->vt = D_089C5688), (self != 0 ? (self->vt = D_089C5650) : 0), (flags & 1))) {
        func_0898D4AC(self, 4);
    }
}
