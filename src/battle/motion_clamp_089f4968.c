typedef struct ClampTask {
    char pad_000[0x8];
    int state;
    char pad_00C[0x4];
    float elapsed;
    char pad_014[0xC];
    char field_20[0x3B0];
    void *anim;
} ClampTask;

extern float func_0889140C(void *anim);
extern void func_089F3D14(void *sub, float time, float length);

int func_089F4968(ClampTask *self, float dt)
{
    float length = func_0889140C(self->anim);
    self->elapsed = self->elapsed + dt;
    if (self->elapsed > length) {
        self->elapsed = length;
    }
    func_089F3D14(self->field_20, self->elapsed, length);
    if (self->elapsed >= length) {
        self->state = 3;
        self->elapsed = 0.0f;
    }
    return 0;
}
