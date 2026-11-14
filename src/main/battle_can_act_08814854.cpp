extern "C" {
struct Actor {
    char pad0[0x380];
    unsigned int flags;
    static unsigned char has(Actor *actor, unsigned int mask) { return (actor->flags & mask) != 0; }
};
struct Battle {
    char pad0[0x30];
    Actor *actor;
};
extern int func_08837990(Actor *actor);
bool func_08814854(Battle *battle)
{
    if (Actor::has(battle->actor, 0x800000) == 1) {
        return false;
    }
    if (Actor::has(battle->actor, 0x4000000) == 1) {
        return false;
    }
    if (func_08837990(battle->actor) == 1) {
        return false;
    }
    return true;
}
}
