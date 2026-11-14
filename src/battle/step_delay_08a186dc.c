typedef struct StepTask {
    void *vtable;
    int field_04;
    int state;
    char pad_0C[0x4];
    float elapsed;
    char part_14[0x28];
    char part_3C[0x4];
} StepTask;

extern float D_08A18C88;
extern int D_08A18C8C;
extern void func_08A01AE0(void *part, int a, int b);
extern void func_08A191C0(void *part);
extern void func_08A19038(void *part, int a, void *data);

/* Waits out the delay, then restarts the parts and moves to state 1. */
int func_08A186DC(StepTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A18C88) {
        func_08A01AE0(self->part_14, 0, -1);
        func_08A191C0(self->part_3C);
        func_08A19038(self->part_3C, 0, &D_08A18C8C);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}
