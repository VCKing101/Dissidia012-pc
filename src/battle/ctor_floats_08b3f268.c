/* Constructs a base object then sets its vtable and two float fields to zero. */
extern void func_08B3D6C0(void *self);
extern char D_08B3FCD8[];
typedef struct { char *vt; float a; float b; } Obj;
Obj *func_08B3F268(Obj *self) {
    func_08B3D6C0(self);
    self->vt = D_08B3FCD8;
    self->a = 0.0f;
    self->b = 0.0f;
    return self;
}
