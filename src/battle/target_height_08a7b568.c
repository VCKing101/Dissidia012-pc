typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct PosBody {
    char pad_00[0x80];
    Vec4 pos;
    char pad_090[0x330];
    float field_3C0;
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

float func_08A7B568(TargetOwner *self)
{
    PosBody *target;
    PosBody *own;
    if (!self->active) {
        return 0.0f;
    }
    target = self->link->target;
    if (target == 0) {
        return 0.0f;
    }
    own = func_08A973A8(self->link);
    if (own == 0) {
        return 0.0f;
    }
    float base = target->pos.y - target->field_3C0;
    return ((float *)&own->pos)[1] - base;
}
