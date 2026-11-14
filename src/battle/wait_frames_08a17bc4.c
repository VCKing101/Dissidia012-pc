typedef struct WaitTask {
    void *vtable;
    int state;
    float elapsed;
    unsigned char skip;
    char pad_0D[0x3];
    int next;
    int frames;
    char pad_18[0xC0];
    char part_D8[0x4];
} WaitTask;

extern float D_08A17CA0;
extern void func_08A08350(void *part);

/* Waits out the frame count (forever when it is the endless value) unless skipped, then moves to state 4. */
int func_08A17BC4(WaitTask *self, float dt)
{
    if ((float)self->frames != D_08A17CA0) {
        self->elapsed = self->elapsed + dt;
    }
    if (self->skip == 1 || self->elapsed >= (float)self->frames) {
        func_08A08350(self->part_D8);
        self->state = 4;
        self->elapsed = 0.0f;
    }
    return 0;
}
