typedef struct TimerLink {
    unsigned char active;
    char pad_01[0x3];
    int id;
    float elapsed;
    float limit;
    void *owner;
    void *target;
} TimerLink;

void func_08AA4D40(TimerLink *self)
{
    self->active = 0;
    self->id = -1;
    self->elapsed = 0.0f;
    self->limit = 0.0f;
    self->owner = 0;
    self->target = 0;
}
