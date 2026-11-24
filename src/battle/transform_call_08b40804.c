/* Converts a rotation to a vector via two helpers and passes it to func_089A383C with w = 1. */
extern void func_08B3B60C(int id, float *out);
extern void func_089A34C0(float *out, float *in);
extern void func_089A383C(float x, float y, float z, float w, int obj);
void func_08B40804(int a0, int obj, int id) {
    float m[4];
    float v[3];
    func_08B3B60C(id, m);
    func_089A34C0(v, m);
    func_089A383C(v[0], v[1], v[2], 1.0f, obj);
}
