/* Resets three vectors via func_089A383C and sets the fields at +0x30..+0x48. */
extern void func_089A383C(float x, float y, float z, float w, void *dst);
typedef struct { char v[0x30]; float f30; float f34; int i38; int i3c; int i40; int i44; int i48; } Obj;
void func_08AEEFC8(Obj *self) {
    float z = 0.0f;
    func_089A383C(z, z, z, z, self);
    func_089A383C(z, z, z, z, self->v + 0x10);
    func_089A383C(z, z, z, z, self->v + 0x20);
    self->f30 = z;
    self->f34 = z;
    self->i38 = 0;
    self->i3c = 2;
    self->i40 = 3;
    self->i44 = 0;
    self->i48 = 0;
}
