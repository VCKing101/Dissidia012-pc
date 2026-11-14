typedef struct TimerTaskF474 {
    char pad_00[0x4];
    int state;
    float elapsed;
    char pad_0C[0xCC];
    char field_D8[0x4];
    char pad_DC[0x41];
    unsigned char field_11D;
} TimerTaskF474;

extern float D_08A0F8F8;
extern void func_08A08350(void *sub);

int func_08A0F474(TimerTaskF474 *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0F8F8) {
        if (self->field_11D == 1) {
            func_08A08350(self->field_D8);
        }
        self->state = 5;
    }
    return 0;
}
