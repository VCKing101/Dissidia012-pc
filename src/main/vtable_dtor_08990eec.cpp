// Deleting destructor: restores the base vtable and frees the object when flags bit 0 is set.
extern "C" int D_089DD370;
extern "C" void func_08990EBC(void *);
struct Obj { char pad[0x904]; int *vt;
  static void Dt(Obj *self, int flags) { if (self) { self->vt = &D_089DD370; if (flags & 1) { func_08990EBC(self); } } }
};
extern "C" void func_08990EEC(Obj *self, int flags) { Obj::Dt(self, flags); }
