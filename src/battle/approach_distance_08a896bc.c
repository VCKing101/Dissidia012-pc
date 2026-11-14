typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

typedef struct Body {
    char pad_00[0x80];
    Vec4 pos;
} Body;

typedef struct ApproachCommand {
    char pad_00[0x4];
    Body *target;
    char pad_08[0x8];
    int id;
} ApproachCommand;

typedef struct ApproachTask {
    void *vtable;
    unsigned char bound;
    char pad_05[0x3];
    ApproachCommand *command;
} ApproachTask;

extern void *func_08AB3264(int id);
extern Body *func_08A973A8(void *slot);
extern void func_089A38FC(Vec4 *out, Vec4 *a, Vec4 *b);
extern float func_089A3BF8(Vec4 *v);

/* Distance from the commanded unit to its target. */
float func_08A896BC(ApproachTask *self)
{
    Body *target;
    Body *own;
    Vec4 delta;

    target = self->command->target;
    if (target == 0) {
        return 0.0f;
    }
    own = func_08A973A8(func_08AB3264(self->command->id));
    if (own == 0) {
        return 0.0f;
    }
    {
        Vec4 *to = &own->pos;
        func_089A38FC(&delta, &target->pos, to);
    }
    return func_089A3BF8(&delta);
}
