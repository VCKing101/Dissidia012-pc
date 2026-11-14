typedef struct TimerTask26830 {
    char pad_00[0x4];
    int state;
    float elapsed;
    char pad_0C[0xD0];
    char field_DC[0x4];
} TimerTask26830;

extern float D_08A268DC;
extern void func_08A08350(void *sub);

int func_08A26830(TimerTask26830 *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A268DC) {
        func_08A08350(self->field_DC);
        self->state = 4;
        self->elapsed = 0.0f;
    }
    return 0;
}
