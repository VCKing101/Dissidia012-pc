typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

typedef struct AimTask {
    char pad_00[0x68];
    int target;
} AimTask;

extern Vec4 D_09AF84E0;
extern int func_08A6AF6C(AimTask *self, Vec4 *dir);
extern int func_089A3B70(Vec4 *v);
extern void func_089A3B30(Vec4 *out, Vec4 *a, Vec4 *b);
extern void func_08A966AC(int target, Vec4 *dir);

int func_08A6B384(AimTask *self)
{
    Vec4 dir;

    if (func_08A6AF6C(self, &dir) != 0) {
        return -1;
    }
    if (func_089A3B70(&dir) == 0) {
        Vec4 *axis = &D_09AF84E0;
        func_089A3B30(&dir, &dir, axis);
        func_08A966AC(self->target, &dir);
    }
    return 0;
}
