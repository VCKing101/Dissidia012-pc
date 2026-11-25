// Destructor of a derived class: set the base then the own vtable, free when asked.
extern "C" char D_089DD320[];
extern "C" char D_089C5730[];
extern "C" void func_08992F04(void *self);
struct Obj { void *vt; };
extern "C" void func_089C514C(Obj *self, unsigned flags)
{
    if (self != 0 && ((self->vt = D_089DD320), (self != 0 ? (self->vt = D_089C5730) : 0), (flags & 1))) {
        func_08992F04(self);
    }
}
