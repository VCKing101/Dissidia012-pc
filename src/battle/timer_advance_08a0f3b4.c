typedef struct TimerTaskF3B4 {
    char pad_00[0x4];
    int state;
    float elapsed;
    char pad_0C[0x98];
    char field_A4[0x4];
    char pad_A8[0x74];
    unsigned char field_11C;
} TimerTaskF3B4;

extern float D_08A0F8F0;
extern void func_08A08350(void *sub);

int func_08A0F3B4(TimerTaskF3B4 *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0F8F0) {
        if (self->field_11C == 1) {
            func_08A08350(self->field_A4);
        }
        self->state = 3;
    }
    return 0;
}
