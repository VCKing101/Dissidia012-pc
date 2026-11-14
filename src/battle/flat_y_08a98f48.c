typedef struct Vec3 {
    float x, y, z;
} Vec3;

extern void func_089A3528(Vec3 *v);
extern float func_089A37DC(Vec3 *v);

/* Length of a vector with its height cleared. */
float func_08A98F48(void)
{
    Vec3 v;

    func_089A3528(&v);
    v.y = 0.0f;
    return func_089A37DC(&v);
}
