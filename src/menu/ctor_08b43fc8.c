/* Constructor: base init, sets vtable pointer, zeroes three floats, stores the owner and registers itself. */
typedef struct O43FC8 { char pad0[4]; int owner; char pad1[4]; void *vt; float f[3]; } O43FC8;
extern void func_08B43DD8(O43FC8 *);
extern void func_08B443D4(O43FC8 *);
extern char D_08B44ED0[];
O43FC8 *func_08B43FC8(O43FC8 *self, int owner)
{
    func_08B43DD8(self);
    self->vt = D_08B44ED0;
    self->f[0] = 0.0f; self->f[1] = 0.0f; self->f[2] = 0.0f;
    self->owner = owner;
    func_08B443D4(self);
    return self;
}
