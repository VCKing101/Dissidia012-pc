/* Tests whether the fighter's attack reaches the given target at its
 * current reach (1 when there is no fighter or the stage allows no reach
 * test, else the reach test's result). */
typedef struct ReachVec4 {
    float x, y, z, w;
} ReachVec4;

typedef struct ReachInfo {
    unsigned char pad_00[4];
    void *fighter;
} ReachInfo;

typedef struct ReachEvent {
    unsigned char pad_00[0x64];
    void *owner;
    unsigned char pad_68[4];
    void *stage;
} ReachEvent;

extern float D_08A7ABFC;

extern "C" {

extern ReachInfo *func_08AB3264(void *owner);
extern void *func_08AB329C(void *owner);
extern int func_08A8707C(void *rules, int id);
extern float func_08837014(void *fighter);
extern int func_08A87B38(void *stage);
extern bool func_08A8435C(void *fighter, float reach, ReachVec4 *point, void *target, int a, int b, int mask,
                          int *hit);

bool func_08A6B180(ReachEvent *self, void *target)
{
    void *fighter;
    float reach;
    ReachVec4 point;
    int hit;
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return true;
    }
    if (func_08A8707C(func_08AB329C(self->owner), 1) == 0) {
        return true;
    }
    reach = D_08A7ABFC * func_08837014(fighter);
    return func_08A8435C(fighter, reach, &point, target, 0, 0, func_08A87B38(self->stage), &hit);
}

}
