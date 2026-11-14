typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x4];
    char field_18[0x4];
} TimerTask;

extern float D_08A16678;
extern void func_08A10DB0(void *sub);

int func_08A16378(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A16678) {
        func_08A10DB0(self->field_18);
        self->state = 1;
    }
    return 0;
}
