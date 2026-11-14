/* Marks the caller's flag (and the event, when it may finish) once the
 * event's timed condition holds (-1 bad arguments, -2 no timing data). */
typedef struct TimeInfo {
    unsigned char pad_00[0xC];
    float scale;
    float time;
} TimeInfo;

typedef struct TimeEvent {
    unsigned char open;
    unsigned char marked;
    unsigned char done;
} TimeEvent;

extern void *D_08A80218;
extern void *func_08A80200(void *battle);
extern TimeInfo *func_08AACB10(void *stage);
extern int func_08A6D02C(TimeEvent *self, float time);
extern int func_08A67614(TimeEvent *self);

int func_08A6F96C(TimeEvent *self, void *key, void *context, unsigned char *flag)
{
    TimeInfo *info;
    if (key == 0 || context == 0 || flag == 0) {
        return -1;
    }
    info = func_08AACB10(func_08A80200(D_08A80218));
    if (info == 0) {
        return -2;
    }
    if (func_08A6D02C(self, info->time) == 1) {
        if (func_08A67614(self)) {
            self->done = 1;
        }
        *flag = 1;
    }
    return 0;
}
