typedef struct ClampEntry {
    char data[0x24];
} ClampEntry;

typedef struct ClampTask19EC0 {
    char pad_000[0x8];
    int state;
    char pad_00C[0x4];
    float elapsed;
    char pad_014[0x50];
    ClampEntry entries[2];
    char pad_0AC[0x27C];
    void *anim;
} ClampTask19EC0;

extern float func_0889140C(void *anim);
extern int func_08A22DEC(float remaining);
extern void func_08A19080(ClampEntry *entry, int value);

int func_08A19EC0(ClampTask19EC0 *self, float dt)
{
    float length = func_0889140C(self->anim);
    int value;
    int i;
    self->elapsed = self->elapsed + dt;
    if (self->elapsed > length) {
        self->elapsed = length;
    }
    value = func_08A22DEC(length - self->elapsed);
    for (i = 0; i < 2; i++) {
        func_08A19080(&self->entries[i], value);
    }
    if (self->elapsed >= length) {
        self->state = 3;
        self->elapsed = 0.0f;
    }
    return 0;
}
