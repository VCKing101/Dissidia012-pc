typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x34];
    char field_48[0x4];
} TimerTask;

extern float D_08A1F6DC;
extern void func_08A10DB0(void *sub);

int func_08A1F45C(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A1F6DC) {
        func_08A10DB0(self->field_48);
        self->state = 2;
    }
    return 0;
}
