typedef struct TimerLink {
    unsigned char active;
    char pad_01[0x3];
    int id;
    float elapsed;
    float limit;
    void *owner;
    void *target;
} TimerLink;

extern void func_08AA4D64(TimerLink *self);
extern int func_08AA4F54(TimerLink *self);

int func_08AA4D80(TimerLink *self, void *owner, void *target)
{
    func_08AA4D64(self);
    if (owner == 0 || target == 0) {
        return -1;
    }
    if (func_08AA4F54(self) != 0) {
        return -2;
    }
    self->owner = owner;
    self->target = target;
    self->active = 1;
    return 0;
}
