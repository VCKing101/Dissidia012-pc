/* Checks the event's reach: the stage object's hit area must accept the
 * event (-1 bad arguments, -2 no fighter, -3 no stage object, -4 refused);
 * the fighter's velocity is measured but both outcomes return 0. */
typedef struct ReachVec {
    float x, y, z, w;
} ReachVec;

typedef struct ReachFighter {
    unsigned char pad_00[0xB0];
    ReachVec velocity;
} ReachFighter;

typedef struct ReachInfo {
    unsigned char pad_00[4];
    ReachFighter *fighter;
} ReachInfo;

typedef struct ReachStage {
    unsigned char pad_00[0x80];
    unsigned char area[4];
} ReachStage;

typedef struct ReachEvent {
    unsigned char pad_00[0x64];
    void *owner;
} ReachEvent;

extern ReachInfo *func_08AB3264(void *owner);
extern ReachStage *func_08A973A8(ReachInfo *info);
extern int func_08A6CB5C(ReachEvent *self, void *area, int mode);
extern float func_089A3BF8(ReachVec *v);

int func_08A70970(ReachEvent *self, void *key, void *context, void *flag)
{
    ReachFighter *fighter;
    ReachStage *stage;
    if (key == 0 || context == 0 || flag == 0) {
        return -1;
    }
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    stage = func_08A973A8(func_08AB3264(self->owner));
    if (stage == 0) {
        return -3;
    }
    if (func_08A6CB5C(self, stage->area, 1) != 0) {
        return -4;
    }
    if (func_089A3BF8(&fighter->velocity) == 0.0f) {
        return 0;
    }
    return 0;
}
