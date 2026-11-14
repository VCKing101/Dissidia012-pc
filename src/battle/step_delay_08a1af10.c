typedef struct StepTask {
    void *vtable;
    int field_04;
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x8];
    char part_1C[0x28];
    char part_44[0x4];
} StepTask;

extern float D_08A1B4A8;
extern int D_08A1B4B0;
extern void func_08A01AE0(void *part, int a, int b);
extern void func_08A191C0(void *part);
extern void func_08A19038(void *part, int a, void *data);

/* Waits out the delay, then restarts the parts and moves to state 1. */
int func_08A1AF10(StepTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A1B4A8) {
        func_08A01AE0(self->part_1C, 0, -1);
        func_08A191C0(self->part_44);
        func_08A19038(self->part_44, 0, &D_08A1B4B0);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}
