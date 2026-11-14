/* Marks the event (cue 0x15) when the fighter's guard condition holds: the
 * air guard while airborne, the ground guard otherwise (clearing the cue
 * when the ground guard fails); -1 bad arguments, -2 no fighter. */
typedef struct GuardInfo {
    unsigned char pad_00[4];
    void *fighter;
} GuardInfo;

typedef struct GuardEvent {
    unsigned char open;
    unsigned char marked;
    unsigned char pad_02[0x62];
    void *owner;
    void *cues;
} GuardEvent;

extern GuardInfo *func_08AB3264(void *owner);
extern int func_088374B0(void *fighter);
extern int func_08A8519C(void *fighter);
extern int func_08A8535C(void *fighter);
extern void func_08A969BC(void *cues, int cue);
extern void func_08A97060(void *cues, int cue);

int func_08A71A98(GuardEvent *self, void *key, void *context, void *flag)
{
    void *fighter;
    if (key == 0 || context == 0 || flag == 0) {
        return -1;
    }
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    if (func_088374B0(fighter) == 0) {
        if (func_08A8519C(fighter) == 0) {
            func_08A969BC(self->cues, 0);
            return 0;
        }
    } else {
        if (func_08A8535C(fighter) == 0) {
            return 0;
        }
    }
    func_08A97060(self->cues, 0x15);
    self->marked = 1;
    return 0;
}
