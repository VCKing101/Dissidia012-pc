// Deleting destructor: restores the base vtable and frees the object when flags bit 0 is set.
extern "C" char D_089DAFC0[];
extern "C" void func_089748D0(void *);
struct Obj { char pad[0x24]; char *vt;
  static void Dt(Obj *self, unsigned flags) { if (self != 0) { self->vt = D_089DAFC0; if (flags & 1) { func_089748D0(self); } } }
};
extern "C" void func_08941CE0(Obj *self, unsigned flags) { Obj::Dt(self, flags); }
