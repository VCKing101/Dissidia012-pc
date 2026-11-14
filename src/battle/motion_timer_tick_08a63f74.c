/* While the owner's motion group is 0x20, counts the timer down; when it
 * runs out, steps the motion to its next entry and restarts the timer with
 * the new entry's length (-1 closed). */
typedef struct TickGroup {
    unsigned char id;
} TickGroup;

typedef struct TickOwner {
    unsigned char pad_000[0x5A4];
    TickGroup *group;
} TickOwner;

typedef struct TickClock {
    unsigned char pad_00[0x18];
    float frame_scale;
} TickClock;

typedef struct MotionTimer {
    unsigned char open;
    unsigned char pad_01[3];
    TickOwner *owner;
    unsigned char pad_08[8];
    float time;
    unsigned char pad_14[0xC];
    unsigned char motion[4];
} MotionTimer;

extern TickClock *D_089D69E0;
extern void *func_08AB327C(void *motion);
extern int func_08AAE300(void *player);
extern int func_08AAE61C(void *player, int entry);
extern void func_08A640CC(MotionTimer *self, int entry);
extern float func_08AB2008(void *player);

int func_08A63F74(MotionTimer *self)
{
    int entry;
    if (!self->open) {
        return -1;
    }
    if (self->owner->group->id != 0x20) {
        return 0;
    }
    self->time -= D_089D69E0->frame_scale;
    if (!(self->time <= 0.0f)) {
        return 0;
    }
    entry = func_08AAE300(func_08AB327C(self->motion));
    func_08A640CC(self, func_08AAE61C(func_08AB327C(self->motion), entry));
    self->time = func_08AB2008(func_08AB327C(self->motion));
    return 0;
}
