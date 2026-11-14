typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

typedef struct Body {
    char pad_00[0x80];
    Vec4 pos;
} Body;

typedef struct BodySlot {
    char pad_00[0x4];
    Body *body;
} BodySlot;

typedef struct DistanceProbe {
    char pad_00[0x64];
    int id;
} DistanceProbe;

extern BodySlot *func_08AB3264(int id);
extern Body *func_08A973A8(BodySlot *slot);
extern void func_089A38FC(Vec4 *out, Vec4 *a, Vec4 *b);
extern float func_089A3BF8(Vec4 *v);

float func_08A66820(DistanceProbe *self)
{
    Body *target;
    Body *own;
    Vec4 delta;

    target = func_08AB3264(self->id)->body;
    if (target == 0) {
        return 0.0f;
    }
    own = func_08A973A8(func_08AB3264(self->id));
    if (own == 0) {
        return 0.0f;
    }
    {
        Vec4 *to = &target->pos;
        func_089A38FC(&delta, &own->pos, to);
    }
    return func_089A3BF8(&delta);
}
