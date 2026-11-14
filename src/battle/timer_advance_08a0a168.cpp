typedef struct TimerEntry {
    char data[0x34];
} TimerEntry;

typedef struct TimerTask {
    char pad_00[0x8];
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0xCC];
    TimerEntry entries[2];
} TimerTask;

extern "C" {

extern float D_08A0A2BC;
extern void func_08A08350(TimerEntry *entry);

int func_08A0A168(TimerTask *self, float dt)
{
    self->elapsed = self->elapsed + dt;
    if (self->elapsed >= D_08A0A2BC) {
        for (int i = 0; i < 2; i++) {
            func_08A08350(&self->entries[i]);
        }
        self->state = 4;
        self->elapsed = 0.0f;
    }
    return 0;
}

}
