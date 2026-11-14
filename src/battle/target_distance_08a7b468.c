typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct PosBody {
    char pad_00[0x80];
    Vec4 pos;
} PosBody;

typedef struct TargetLink {
    char pad_00[0x4];
    PosBody *target;
} TargetLink;

typedef struct TargetOwner {
    unsigned char active;
    char pad_01[0x3];
    TargetLink *link;
} TargetOwner;

extern PosBody *func_08A973A8(TargetLink *link);
extern void func_089A38FC(Vec4 *out, Vec4 *a, Vec4 *b);
extern float func_089A3BF8(Vec4 *v);

float func_08A7B468(TargetOwner *self)
{
    float zero = 0.0f;
    PosBody *target;
    PosBody *own;
    Vec4 offset;
    if (!self->active) {
        return zero;
    }
    target = self->link->target;
    if (target == 0) {
        return zero;
    }
    own = func_08A973A8(self->link);
    if (own == 0) {
        return zero;
    }
    func_089A38FC(&offset, &own->pos, &target->pos);
    offset.y = zero;
    return func_089A3BF8(&offset);
}
