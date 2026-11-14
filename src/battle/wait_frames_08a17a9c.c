typedef struct WaitTask {
    void *vtable;
    int state;
    float elapsed;
    unsigned char skip;
    char pad_0D[0x3];
    int next;
    int frames;
    char pad_18[0x38];
    char part_50[0x4];
} WaitTask;

extern float D_08A17CA0;
extern void func_08A17288(void *part, int next);

/* Waits out the frame count (forever when it is the endless value) unless skipped, then moves on. */
int func_08A17A9C(WaitTask *self, float dt)
{
    if ((float)self->frames != D_08A17CA0) {
        self->elapsed = self->elapsed + dt;
    }
    if (self->skip == 1 || self->elapsed >= (float)self->frames) {
        func_08A17288(self->part_50, self->next);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}
