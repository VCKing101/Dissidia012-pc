typedef struct Vec466744 {
    float x;
    float y;
    float z;
    float w;
} Vec466744;

typedef struct Unit66744 {
    char pad_000[0x80];
    Vec466744 position;
} Unit66744;

typedef struct UnitSlot66744 {
    char pad_000[0x4];
    Unit66744 *unit;
} UnitSlot66744;

typedef struct UnitOwner66744 {
    char pad_000[0x64];
    int field_64;
} UnitOwner66744;

extern void *func_08A6AF38(UnitOwner66744 *self);
extern float func_0884E474(void *handle);
extern float func_0884E47C(void *handle);
extern UnitSlot66744 *func_08AB3264(int id);
extern Unit66744 *func_08AB32B8(Unit66744 *unit);
extern void func_089A38FC(Vec466744 *out, Vec466744 *a, Vec466744 *b);
extern float func_089A3BF8(Vec466744 *v);

float func_08A66744(UnitOwner66744 *self)
{
    void *handle = func_08A6AF38(self);
    if (!handle) {
        return 0.0f;
    }
    return func_0884E474(handle);
}

float func_08A6677C(UnitOwner66744 *self)
{
    void *handle = func_08A6AF38(self);
    if (!handle) {
        return 0.0f;
    }
    return func_0884E47C(handle);
}

float func_08A667B4(UnitOwner66744 *self)
{
    Unit66744 *unit = func_08AB3264(self->field_64)->unit;
    Unit66744 *target;
    float distance;
    if (!unit) {
        return 0.0f;
    }
    target = func_08AB32B8(unit);
    if (!target) {
        return 0.0f;
    }
    {
        Vec466744 delta;
        func_089A38FC(&delta, &target->position, &unit->position);
        distance = func_089A3BF8(&delta);
    }
    return distance;
}
