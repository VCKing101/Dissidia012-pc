/* Event readiness: reads the event's two hit flags (-2 when that fails),
 * then raises the act cue when the fighter may act, the step cue when the
 * step check passes, and marks the event when the owner is in state 10
 * (-1 bad arguments). */
typedef struct ReadyEvent {
    unsigned char open;
    unsigned char marked;
    unsigned char pad_02[0x4C];
    unsigned char step;
    unsigned char pad_4f[0x15];
    void *owner;
    void *cues;
} ReadyEvent;

extern int func_08A6CCB4(ReadyEvent *self, int a, int b, unsigned char *hit, unsigned char *guard, int c);
extern unsigned char func_08A6B904(ReadyEvent *self, unsigned char check);
extern void func_08A969BC(void *cues, int cue);
extern int func_08A6B9D8(ReadyEvent *self, unsigned char step);
extern void func_08A97098(void *cues, int cue);
extern void *func_08AB3274(void *owner);
extern int func_08A82638(void *state, int which);

int func_08A71DD8(ReadyEvent *self, void *key, void *context, void *flag)
{
    unsigned char hit;
    unsigned char guard;
    if (key == 0 || context == 0 || flag == 0) {
        return -1;
    }
    hit = 0;
    guard = 0;
    if (func_08A6CCB4(self, 0, 1, &hit, &guard, 1) != 0) {
        return -2;
    }
    unsigned char act = func_08A6B904(self, 0);
    int one = 1;
    if (act == one) {
        func_08A969BC(self->cues, 1);
    }
    if (func_08A6B9D8(self, self->step) == one) {
        func_08A97098(self->cues, 0xD);
    }
    if (func_08A82638(func_08AB3274(self->owner), 0xA) == one) {
        self->marked = 1;
    }
    return 0;
}
