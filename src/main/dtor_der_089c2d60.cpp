// Destructor of a derived class: set the base then the own vtable, free when asked.
extern "C" char D_089C5570[];
extern "C" char D_089C5468[];
extern "C" void func_089748D0(void *self);
struct Obj { int pad; void *vt; };
extern "C" void func_089C2D60(Obj *self, unsigned flags)
{
    if (self != 0) {
        self->vt = D_089C5570;
        if (self != 0) {
            self->vt = D_089C5468;
        }
        if (flags & 1) {
            func_089748D0(self);
        }
    }
}
