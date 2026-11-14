typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;

    static void set(Vec4 *v, float x, float y, float z, float w)
    {
        v->x = x;
        v->y = y;
        v->z = z;
        v->w = w;
    }
} Vec4;

typedef struct AimShape {
    char pad_00[0x4];
    float scale;
    char pad_08[0x28];
    float x;
    char pad_34[0x4];
    float z;
} AimShape;

typedef struct AimParams {
    char pad_00[0xAC];
    float y;
    float scale;
} AimParams;

typedef struct AimUnit {
    char pad_000[0x384];
    unsigned int flags;
    char pad_388[0x14];
    AimParams *params;
    char pad_3A0[0x8];
    AimShape *shape;
} AimUnit;

extern "C" {

extern void func_08837A38(AimUnit *unit);
extern void func_08A5BF84(AimUnit *unit, Vec4 *target);
extern void func_08834C10(AimUnit *unit, int a, int b);

void func_08A5C374(void *self, AimUnit *unit)
{
    AimShape *shape = unit->shape;
    AimParams *params = unit->params;
    Vec4 target;
    unit->flags |= 2;
    func_08837A38(unit);
    Vec4::set(&target, shape->x, params->y, shape->z, shape->scale * params->scale);
    func_08A5BF84(unit, &target);
    func_08834C10(unit, 1, 0);
}

}
