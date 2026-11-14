typedef struct RangeEvent {
    char pad_00[0x8];
    int kind;
    float range;
} RangeEvent;

typedef struct RangeWatcher {
    unsigned char active;
    unsigned char inRange;
} RangeWatcher;

extern float func_08A66820(RangeWatcher *self);

/* Event kind 4: sets inRange when the target is closer than the range. */
int func_08A6E958(RangeWatcher *self, RangeEvent *event, void *arg2, void *arg3)
{
    if (event == 0 || arg2 == 0 || arg3 == 0) {
        return -1;
    }
    if (event->kind != 4) {
        return -2;
    }
    if (func_08A66820(self) < event->range) {
        unsigned char one = 1;
        self->inRange = one;
    }
    return 0;
}
