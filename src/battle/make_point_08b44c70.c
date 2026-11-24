/* Builds a point from fields of the target's record, transforms it with func_08899780 (scale 20) and returns the result vector. */
extern void func_08899780(float s, float *out, float *in);
typedef struct { float x, y, z, w; } Vec4;
typedef struct { char pad[0x10]; float a; float b; } Rec;
typedef struct { int pad; Rec *rec; } Src;
void func_08B44C70(Vec4 *out, Src *src) {
    Vec4 in __attribute__((aligned(16)));
    Vec4 res __attribute__((aligned(16)));
    float a = src->rec->a;
    float b = src->rec->b;
    in.x = a;
    in.y = 0.0f;
    in.z = b;
    in.w = 1.0f;
    func_08899780(20.0f, &res.x, &in.x);
    *out = res;
}
