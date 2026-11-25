// Destructor of a derived class: set the base then the own vtable, free when asked.
extern "C" char D_089DD2E8[];
extern "C" char D_089C5630[];
extern "C" void func_08992F04(void *self);
struct Obj { void *vt; };
extern "C" void func_089C4F04(Obj *self, unsigned flags)
{
    if (self != 0 && ((self->vt = D_089DD2E8), (self != 0 ? (self->vt = D_089C5630) : 0), (flags & 1))) {
        func_08992F04(self);
    }
}
