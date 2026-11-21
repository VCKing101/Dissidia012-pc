/* AI condition 4 (approach on timer 31): inside the near band of the event
 * clock the approach is off (it stops and the count restarts), inside the
 * far band on; while on, counts the fighter's frames and is met at the
 * condition's limit, else keeps approaching (-1 bad arguments, -2 other
 * condition, -3 no fighter). */
typedef struct NearCond {
    unsigned char pad_00[8];
    int kind;
    float limit;
} NearCond;

typedef struct NearAi {
    unsigned char pad_000;
    unsigned char met;
    unsigned char pad_002[2];
    unsigned char timers[0x60];
    void *owner;
    unsigned char pad_068[0x134];
    unsigned char on;
    unsigned char pad_19d[3];
    float count;
} NearAi;

typedef struct NearInfo {
    unsigned char pad_00[4];
    void *fighter;
} NearInfo;

extern float D_08A7AC2C;
extern float D_08A7AC28;

extern int func_08A7E904(void *timers, int id);
extern void func_08A7E858(void *timers, int id);
extern NearInfo *func_08AB3264(void *owner);
extern float func_08A66820(NearAi *self);
extern void func_08A6C68C(NearAi *self, int order);
extern float func_08838300(void *fighter);
extern void func_08A6B0A0(NearAi *self);

int func_08A71694(NearAi *self, NearCond *cond, void *key, void *flag)
{
    if (cond == 0 || key == 0 || flag == 0) {
        return -1;
    }
    if (cond->kind != 4) {
        return -2;
    }
    {
        int id = 0x1F;
        void *timers = self->timers;
        void *fighter;
        float now;
        if (func_08A7E904(timers, id) == 0) {
            self->on = 1;
            self->count = 0.0f;
        }
        func_08A7E858(timers, id);
        fighter = func_08AB3264(self->owner)->fighter;
        if (fighter == 0) {
            return -3;
        }
        now = func_08A66820(self);
        if (D_08A7AC2C < now) {
            self->on = 0;
        } else if (!(D_08A7AC28 <= now)) {
            self->on = 1;
        }
        if (self->on == 0) {
            func_08A6C68C(self, 7);
            self->count = 0.0f;
        } else {
            self->count = self->count + func_08838300(fighter);
            if (!(self->count < cond->limit)) {
                self->met = 1;
            } else {
                func_08A6B0A0(self);
            }
        }
    }
    return 0;
}
