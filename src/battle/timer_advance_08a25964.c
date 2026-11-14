typedef struct TimerTask25964 {
    char pad_00[0x4];
    int state;
    float elapsed;
    int field_0C;
    int field_10;
    char pad_14[0x38];
    char field_4C[0x4];
} TimerTask25964;

extern float D_08A25AE8;
extern void func_08A25F60(void *sub, int arg1, int arg2);

int func_08A25964(TimerTask25964 *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A25AE8) {
        func_08A25F60(self->field_4C, self->field_0C, self->field_10);
        self->state = 1;
        self->elapsed = 0.0f;
    }
    return 0;
}
