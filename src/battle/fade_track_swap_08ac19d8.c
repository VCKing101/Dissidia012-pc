typedef struct FadePose {
    char pad_00[0xC];
    float duration;
} FadePose;

typedef struct FadeUnit {
    char pad_00[0x48];
    FadePose *pose;
} FadeUnit;

typedef struct FadeTrack {
    int pad_00;
    FadeUnit *unit;
    char pad_08[0x8];
    int state;
    void *task;
    void *next;
    float time;
} FadeTrack;

extern void func_08AB6A80(void *task);
extern void func_08AB6B30(void *task);

/* Advance the track; at the pose's end stop the task, start the queued one and enter state 2. */
void func_08AC19D8(FadeTrack *self, float dt)
{
    float limit = self->unit->pose->duration;
    self->time = self->time + dt;
    if (self->time > limit) {
        self->time = limit;
    }
    if (self->time == limit) {
        func_08AB6A80(self->task);
        self->task = self->next;
        self->next = 0;
        func_08AB6B30(self->task);
        self->state = 2;
    }
}
