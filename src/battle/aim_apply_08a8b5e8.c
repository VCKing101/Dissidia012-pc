typedef struct Vec4 {
    float x, y, z, w;
} Vec4;

typedef struct AimCommand {
    char pad_00[0x8];
    int target;
} AimCommand;

typedef struct AimTask {
    void *vtable;
    unsigned char bound;
    char pad_05[0x3];
    AimCommand *command;
} AimTask;

extern Vec4 func_08A8B538(AimTask *self, int a, int b);
extern void func_08A966AC(int target, Vec4 *dir);

void func_08A8B5E8(AimTask *self, int a, int b)
{
    int target = self->command->target;
    Vec4 dir;

    dir = func_08A8B538(self, a, b);
    func_08A966AC(target, &dir);
}
