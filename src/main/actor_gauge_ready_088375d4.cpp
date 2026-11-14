extern "C" {
typedef struct ActorSub { char pad_0[2]; unsigned char flags; } ActorSub;
typedef struct Gauge { char pad_0[4]; short value; } Gauge;
typedef struct Actor088375D4 { char pad_000[0x190]; ActorSub sub[1]; char pad_193[0x388 - 0x193]; unsigned long long flags; char pad_390[0x468 - 0x390]; Gauge *gauge; } Actor088375D4;
extern unsigned long long D_089C7F10;
struct ActorOps {
    static bool has(Actor088375D4 *a) { return (a->flags & D_089C7F10) != 0; }
    static bool subHas(ActorSub *s, int bit) { return (s->flags & bit) != 0; }
    static bool positive(Gauge *g) { return g->value > 0; }
};
unsigned char func_088375D4(Actor088375D4 *a)
{
    int r = 0;
    if (ActorOps::has(a) && !ActorOps::subHas(a->sub, 2) && a->gauge != 0 && ActorOps::positive(a->gauge)) {
        r = 1;
    }
    return (unsigned char)r;
}
}
