typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

typedef struct AimUnit {
    char pad_000[0x100];
    Vec4 pos;
} AimUnit;

typedef struct AimSlot {
    char pad_00[0x4];
    AimUnit *body;
} AimSlot;

typedef struct AimTask {
    char pad_00[0x64];
    int id;
    char pad_68[0x4];
    int view;
} AimTask;

extern float D_08A7ABF8;
extern AimSlot *func_08AB3264(int id);
extern int func_089A3B70(Vec4 *dir);
extern void func_089A3A64(Vec4 *out, Vec4 *dir, float scale);
extern void func_089A38C4(Vec4 *out, Vec4 *a, Vec4 *b);
extern void func_08A877F8(int view, Vec4 *pos, unsigned int slot, int flags, float x, float y);

/* Points view slot at the unit's position moved along dir; -1 bad slot, -2 no unit. */
int func_08A6C850(AimTask *self, Vec4 *dir, unsigned int slot)
{
    AimUnit *body;
    Vec4 pos;
    if (slot >= 8) {
        return -1;
    }
    body = func_08AB3264(self->id)->body;
    if (body == 0) {
        return -2;
    }
    if (func_089A3B70(dir) == 1) {
        return 0;
    }
    {
        Vec4 *origin = &body->pos;
        func_089A3A64(&pos, dir, D_08A7ABF8);
        func_089A38C4(&pos, &pos, origin);
    }
    func_08A877F8(self->view, &pos, slot, 0, 0.0f, 0.0f);
    return 0;
}
