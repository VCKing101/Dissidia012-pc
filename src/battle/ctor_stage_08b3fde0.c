/* Constructs a base object, sets its vtable and zeroed float and int fields, then runs func_08B4024C. */
extern void func_08B3D6C0(void *self);
extern void func_08B4024C(void *self, int a);
extern char D_08B41560[];
typedef struct { char *vt; int p1; float f2; int p3, p4; float f5; int p6; int i7; } Obj;
Obj *func_08B3FDE0(Obj *self, int a) {
    func_08B3D6C0(self);
    self->vt = D_08B41560;
    self->f2 = 0.0f;
    self->f5 = 0.0f;
    self->i7 = 0;
    func_08B4024C(self, a);
    return self;
}
