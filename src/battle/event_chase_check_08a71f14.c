/* While the stage object runs its chase motions (0x385, 0x38F): updates
 * the event and clears the cue once the fighter is behind it; otherwise
 * marks the event done (-1 bad arguments, -2 no fighter, -3 no stage
 * object). */
typedef struct ChaseBody {
    unsigned char pad_000[0x84];
    float distance;
    unsigned char pad_088[0xF8];
    short motion_id;
} ChaseBody;

typedef struct ChaseInfo {
    unsigned char pad_00[4];
    ChaseBody *fighter;
} ChaseInfo;

typedef struct ChaseEvent {
    unsigned char open;
    unsigned char marked;
    unsigned char done;
    unsigned char pad_03[0x61];
    void *owner;
    void *cues;
} ChaseEvent;

extern ChaseInfo *func_08AB3264(void *owner);
extern ChaseBody *func_08A973A8(ChaseInfo *info);
extern void func_08A6B23C(ChaseEvent *self);
extern void func_08A969BC(void *cues, int cue);

int func_08A71F14(ChaseEvent *self, void *key, void *context, void *flag)
{
    ChaseBody *fighter;
    ChaseBody *stage;
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
    if (stage->motion_id != 0x385 && stage->motion_id != 0x38F) {
        self->done = 1;
    } else {
        func_08A6B23C(self);
        if (fighter->distance < stage->distance) {
            func_08A969BC(self->cues, 0);
        }
    }
    return 0;
}
