/* Clears both corners of a bounds record and marks it empty. */
typedef struct Vec4_088738B8 {
    float x;
    float y;
    float z;
    float w;
} Vec4_088738B8;

typedef struct Bounds088738B8 {
    Vec4_088738B8 min;
    Vec4_088738B8 max;
    char pad_20[0x214];
    int count;
    unsigned char empty;
} Bounds088738B8;

void func_089A383C(Vec4_088738B8 *v, float x, float y, float z, float w);

void func_088738B8(Bounds088738B8 *b)
{
    float zero = 0.0f;
    func_089A383C(&b->min, zero, zero, zero, zero);
    func_089A383C(&b->max, zero, zero, zero, zero);
    b->count = 0;
    b->empty = 1;
}
