/* Sets two floats, builds an offset vector from a global scale and passes it to a setter, then sets two flag bits. */
typedef struct V3 { float x, y, z; } V3;
typedef struct O43F24 { char pad0[8]; int flags; char pad1[0x1C - 0xC]; char sub[0x10]; float a; float b; } O43F24;
extern void func_089A34C0(void *, V3 *);
extern float D_08B44EF0;
void func_08B43F24(O43F24 *self, float b)
{
    float zero = 0.0f;
    V3 v;
    self->a = zero;
    self->b = b;
    v.x = D_08B44EF0 * -40.0f;
    v.y = zero;
    v.z = zero;
    func_089A34C0(self->sub, &v);
    self->flags |= 1;
    self->flags |= 2;
}
