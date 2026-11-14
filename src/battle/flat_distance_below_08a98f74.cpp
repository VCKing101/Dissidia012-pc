typedef struct Vec3 {
    float x, y, z;
} Vec3;

extern "C" {
extern void func_089A3528(Vec3 *v);
extern float func_089A37DC(Vec3 *v);
}

struct FlatVector {
    static float length()
    {
        Vec3 v;
        func_089A3528(&v);
        v.y = 0.0f;
        return func_089A37DC(&v);
    }
};

/* True when the horizontal length is below the limit. */
extern "C" unsigned char func_08A98F74(float limit)
{
    float length = FlatVector::length();
    return (unsigned char)(length < limit ? 1 : 0);
}
