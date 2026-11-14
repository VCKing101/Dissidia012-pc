typedef struct ValueRange {
    char pad_00[0x4];
    int min;
    int max;
} ValueRange;

typedef struct TimerLink {
    unsigned char active;
    char pad_01[0x3];
    int id;
    float elapsed;
    float ratio;
    void *owner;
    int target;
} TimerLink;

extern void *func_08AB327C(int id);
extern ValueRange *func_08AB2900(void *unit);

/* Picks the value at the link's ratio inside the target's range. */
int func_08AA4F88(TimerLink *self)
{
    ValueRange *range = func_08AB2900(func_08AB327C(self->target));

    if (range == 0) {
        return 0;
    }
    return (int)((float)(range->max - range->min + 1) * self->ratio) + range->min;
}
