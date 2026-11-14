typedef struct ScoreState {
    unsigned char active;
    char pad_01[0xF];
    int state;
    char pad_14[0x4];
    void *handle;
    float time;
    char pad_20[0x30];
    int base;
    char pad_54[0x8];
    int bonus;
    char pad_60[0x70];
    int total;
    int shown;
} ScoreState;

extern void func_08AB6A80(void *handle);

/* Releases the handle and moves to state 4 with the total score; -1 inactive. */
int func_08AC1598(ScoreState *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->handle != 0) {
        func_08AB6A80(self->handle);
        self->handle = 0;
    }
    self->state = 4;
    self->time = 0.0f;
    self->total = self->base + self->bonus;
    self->shown = 0;
    return 0;
}
