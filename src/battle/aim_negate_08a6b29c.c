typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

typedef struct AimTask {
    char pad_00[0x68];
    int target;
} AimTask;

extern int func_08A6AF6C(AimTask *self, Vec4 *dir);
extern int func_089A3B70(Vec4 *v);
extern void func_089A3A64(float scale, Vec4 *out, Vec4 *v);
extern void func_08A966AC(int target, Vec4 *dir);

/* Points the target along the reversed aim direction. */
int func_08A6B29C(AimTask *self)
{
    Vec4 dir;

    if (func_08A6AF6C(self, &dir) != 0) {
        return -1;
    }
    if (func_089A3B70(&dir) == 0) {
        float scale = -1.0f;
        func_089A3A64(scale, &dir, &dir);
        func_08A966AC(self->target, &dir);
    }
    return 0;
}
