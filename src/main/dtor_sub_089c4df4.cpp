// Destructor: set the vtable at +0x14, run the member cleanup and free the object when asked.
extern "C" char D_089DD250[];
extern "C" void func_089843EC(void *self, int arg);
extern "C" void func_0898D4AC(void *self, int size);
struct Obj { char pad[0x14]; void *vt; };
struct H {
    static void run(Obj *self, unsigned flags)
    {
        if (self != 0) {
            self->vt = D_089DD250;
            func_089843EC(self, 0);
            if (flags & 1) {
                func_0898D4AC(self, 0x18);
            }
        }
    }
};
extern "C" void func_089C4DF4(Obj *self, unsigned flags) { H::run(self, flags); }
