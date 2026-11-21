/* AI condition 4 (time on timer 17): counts the fighter's frames since the
 * timer started and sets the condition's met flag once the count reaches
 * the condition's limit (-1 bad arguments, -2 other condition, -3 no
 * fighter). */
typedef struct TimeCond {
    unsigned char pad_00[8];
    int kind;
    float limit;
} TimeCond;

typedef struct TimeAi {
    unsigned char pad_000;
    unsigned char met;
    unsigned char met2;
    unsigned char pad_003;
    unsigned char timers[0x60];
    void *owner;
    unsigned char pad_068[0xB0];
    float count;
    float count2;
} TimeAi;

typedef struct TimeInfo {
    unsigned char pad_00[4];
    void *fighter;
} TimeInfo;

extern int func_08A7E904(void *timers, int id);
extern void func_08A7E858(void *timers, int id);
extern TimeInfo *func_08AB3264(void *owner);
extern float func_08838300(void *fighter);

int func_08A6E6A0(TimeAi *self, TimeCond *cond, void *key, void *flag)
{
    if (cond == 0 || key == 0 || flag == 0) {
        return -1;
    }
    if (cond->kind != 4) {
        return -2;
    }
    {
        int fresh = 0;
        int id = 0x11;
        void *timers = self->timers;
        void *fighter;
        if (func_08A7E904(timers, id) == 0) {
            self->count = 0.0f;
            fresh = 1;
        }
        func_08A7E858(timers, id);
        fighter = func_08AB3264(self->owner)->fighter;
        if (fighter == 0) {
            return -3;
        }
        if (!fresh) {
            self->count = self->count + func_08838300(fighter);
        }
        if (!(self->count < cond->limit)) {
            self->met = 1;
        }
    }
    return 0;
}
