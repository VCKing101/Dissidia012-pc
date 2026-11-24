/* If a target is set at +0xC, passes the vector (5000, 1000, 1000, 1) and colour 0xFFBFBF9F to func_08B53C20. */
struct Obj { char pad[0xC]; void *target; };
extern void func_08B53C20(void *, float *, unsigned);
void func_08B4EE68(struct Obj *self)
{
    float v[4];
    v[0] = 5000.0f;
    v[1] = 1000.0f;
    v[2] = 1000.0f;
    v[3] = 1.0f;
    if (self->target == 0) {
        return;
    }
    func_08B53C20(self->target, v, 0xFFBFBF9F);
}
