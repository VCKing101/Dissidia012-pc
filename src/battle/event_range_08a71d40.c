typedef struct RangeEvent {
    char pad_00[0x8];
    int kind;
    float range;
} RangeEvent;

typedef struct RangeWatcher {
    unsigned char active;
    unsigned char inRange;
    char pad_02[0x62];
    int id;
} RangeWatcher;

extern void *func_08AB328C(int id);
extern float func_08A7B468(void *unit);

/* Event kind 4: sets inRange when the unit's distance is within the range. */
int func_08A71D40(RangeWatcher *self, RangeEvent *event, void *arg2, void *arg3)
{
    if (event == 0 || arg2 == 0 || arg3 == 0) {
        return -1;
    }
    if (event->kind != 4) {
        return -2;
    }
    if (func_08A7B468(func_08AB328C(self->id)) <= event->range) {
        unsigned char one = 1;
        self->inRange = one;
    }
    return 0;
}
