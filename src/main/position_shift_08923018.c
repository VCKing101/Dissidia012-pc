/* fetch a position and lower its x by a quarter of a global. */
typedef struct V3 { float x, y, z; } V3;
extern void func_08922F9C(V3 *v);
extern float D_08B4BFD8;
void func_08923018(V3 *out)
{
    V3 v;
    func_08922F9C(&v);
    v.x = v.x - D_08B4BFD8 * 0.25f;
    *out = v;
}
