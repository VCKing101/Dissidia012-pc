typedef struct TimerLink {
    unsigned char active;
    char pad_01[0x3];
    int id;
    float elapsed;
    float limit;
    void *owner;
    void *target;
} TimerLink;

extern int D_08AA5828;
extern float func_08AA57D0(int id);

int func_08AA4F54(TimerLink *self)
{
    self->limit = func_08AA57D0(D_08AA5828);
    return 0;
}
