/* AI condition 4 (time on timer 27): the countdown starts at the current
 * motion's length and runs while the fighter is free; once the event
 * clock is within the condition's limit or the countdown is out, and the
 * fighter is on its target's side or cannot turn, the condition is met
 * when the fighter is free (-1 bad arguments, -2 other condition, -3 no
 * fighter). */
typedef struct LeftCond {
    unsigned char pad_00[8];
    int kind;
    float limit;
} LeftCond;

typedef struct LeftAi {
    unsigned char pad_000;
    unsigned char met;
    unsigned char pad_002[2];
    unsigned char timers[0x60];
    void *owner;
    unsigned char pad_068[4];
    void *orders;
    unsigned char pad_070[0xF4];
    float left;
} LeftAi;

typedef struct LeftInfo {
    unsigned char pad_00[4];
    void *fighter;
} LeftInfo;

extern int func_08A7E904(void *timers, int id);
extern void func_08A7E858(void *timers, int id);
extern void *func_08AB327C(void *owner);
extern float func_08AAF3DC(void *motion);
extern LeftInfo *func_08AB3264(void *owner);
extern float func_08A66820(LeftAi *self);
extern int func_088377C0(void *fighter);
extern float func_08838300(void *fighter);
extern int func_08A87AF8(void *orders);
extern int func_08A87B18(void *orders);
extern int func_08A84024(void *fighter);

int func_08A6FBFC(LeftAi *self, LeftCond *cond, void *key, void *flag)
{
    if (cond == 0 || key == 0 || flag == 0) {
        return -1;
    }
    if (cond->kind != 4) {
        return -2;
    }
    {
        int id = 0x1B;
        void *timers = self->timers;
        void *fighter;
        float now;
        if (func_08A7E904(timers, id) == 0) {
            self->left = func_08AAF3DC(func_08AB327C(self->owner));
        }
        func_08A7E858(timers, id);
        fighter = func_08AB3264(self->owner)->fighter;
        if (fighter == 0) {
            return -3;
        }
        now = func_08A66820(self);
        if (func_088377C0(fighter) == 0) {
            self->left = self->left - func_08838300(fighter);
        }
        if (now <= cond->limit || self->left <= 0.0f) {
            int side = func_08A87AF8(self->orders);
            if (side == func_08A87B18(self->orders) || func_08A84024(fighter) == 0) {
                if (func_088377C0(fighter) == 0) {
                    self->met = 1;
                }
            }
        }
    }
    return 0;
}
