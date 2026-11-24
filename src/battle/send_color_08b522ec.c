/* Builds the vector (5000, 800, 1000, 1) and passes it with the object's field +0x34 and colour 0xFF6CA9B7 to func_08B53C20. */
struct Obj { char pad[0x34]; void *target; };
extern void func_08B53C20(void *, float *, unsigned);
void func_08B522EC(struct Obj *self)
{
    float v[4];
    v[0] = 5000.0f;
    v[1] = 800.0f;
    v[2] = 1000.0f;
    v[3] = 1.0f;
    func_08B53C20(self->target, v, 0xFF6CA9B7);
}
