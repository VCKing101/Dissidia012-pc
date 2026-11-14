extern "C" {
struct Motion {
    char pad0[0x8];
    int playing;
    char padC[0xC];
    int state;
};
struct Actor {
    char pad0[0x568];
    Motion *motion;
    char pad56C[0x34];
    signed char visible;
};
struct Target {
    char pad0[0x14];
    Actor *actor;
};
bool func_0881D5A0(int unused, Target *target)
{
    Actor *actor;
    Motion *motion;
    if (target == 0) {
        return false;
    }
    actor = target->actor;
    if (actor == 0) {
        return false;
    }
    if (actor->visible == 0) {
        return false;
    }
    motion = actor->motion;
    if (motion == 0) {
        return false;
    }
    if (motion->playing == 0) {
        return false;
    }
    if (motion->state == 1) {
        return false;
    }
    return true;
}
}
