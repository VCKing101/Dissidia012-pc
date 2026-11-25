/* Constructor: base init, sets vtable pointer, zeroes nine floats, stores the owner and registers itself. */
typedef struct O43E8C { char pad0[4]; int owner; char pad1[4]; void *vt; float f[9]; } O43E8C;
extern void func_08B43DD8(O43E8C *);
extern void func_08B43F90(O43E8C *);
extern char D_08B44EB0[];
O43E8C *func_08B43E8C(O43E8C *self, int owner)
{
    func_08B43DD8(self);
    self->vt = D_08B44EB0;
    self->f[0] = 0.0f; self->f[1] = 0.0f; self->f[2] = 0.0f;
    self->f[3] = 0.0f; self->f[4] = 0.0f; self->f[5] = 0.0f;
    self->f[6] = 0.0f; self->f[7] = 0.0f; self->f[8] = 0.0f;
    self->owner = owner;
    func_08B43F90(self);
    return self;
}
