typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

extern void func_089A38FC(Vec4 *out, Vec4 *a, Vec4 *b);
extern float func_089A3B10(Vec4 *a, Vec4 *b);

int func_08A45BD8(void *self, Vec4 *origin, Vec4 *dir, Vec4 *center, float radius)
{
    Vec4 offset;
    float c;
    float b;
    func_089A38FC(&offset, origin, center);
    c = func_089A3B10(&offset, &offset) - radius * radius;
    if (c <= 0.0f) {
        return 1;
    }
    b = func_089A3B10(&offset, dir);
    if (!(b <= 0.0f)) {
        return 0;
    }
    if (b * b - c < 0.0f) {
        return 0;
    }
    return 1;
}
