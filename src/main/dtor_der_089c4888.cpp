// Destructor of a derived class: set the base then the own vtable, free when asked.
extern "C" char D_089DD108[];
extern "C" char D_089C55E0[];
extern "C" void func_08992F04(void *self);
struct Obj { void *vt; };
extern "C" void func_089C4888(Obj *self, unsigned flags)
{
    if (self != 0 && ((self->vt = D_089DD108), (self != 0 ? (self->vt = D_089C55E0) : 0), (flags & 1))) {
        func_08992F04(self);
    }
}
