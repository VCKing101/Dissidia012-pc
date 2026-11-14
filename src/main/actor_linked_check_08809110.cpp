extern "C" {
struct Actor {
    char pad0[0x542];
    unsigned char kind;
    char pad543[0x91];
    Actor *leader;
    static unsigned char isLinked(Actor *actor) { return actor->kind == 3; }
};
struct Battle {
    char pad0[0x18];
    Actor *player;
};
int func_08809110(Battle **battle, Actor *actor)
{
    Actor *player = (*battle)->player;
    if (player == actor) {
        return 1;
    }
    if (Actor::isLinked(actor)) {
        if (player == actor->leader) {
            return 1;
        }
        if (player != 0 && Actor::isLinked(player) && player->leader == actor->leader) {
            return 1;
        }
    }
    return 0;
}
}
