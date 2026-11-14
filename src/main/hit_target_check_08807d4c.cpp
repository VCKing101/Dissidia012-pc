/* Whether an attack may hit the actor: the actor must be hittable, inside
   the attack's area, and not on the attacker's own side (both resolved
   through summon and partner links to the controlling actor). */
struct Actor08807D4C;

struct Body08807D4C {
    char pad_00[0x28];
    char area[1];
};

struct Actor08807D4C {
    char pad_000[0x3AC];
    Body08807D4C *body;
    char pad_3B0[0x542 - 0x3B0];
    unsigned char kind;
    char pad_543[0x5D4 - 0x543];
    Actor08807D4C *summoner;
    char pad_5D8[0x1108 - 0x5D8];
    Actor08807D4C *partner;

    static bool is_kind(Actor08807D4C *a, int kind) { return a->kind == kind; }

    static Actor08807D4C *controller(Actor08807D4C *a)
    {
        if (a != 0 && is_kind(a, 3) == true) {
            a = a->summoner;
        }
        if (a != 0 && is_kind(a, 1) == true) {
            a = a->partner;
        }
        return a;
    }
};

struct Attack08807D4C {
    char pad_00[0x18];
    Actor08807D4C *owner;
};

extern "C" {

int func_088378C4(Actor08807D4C *actor);
int func_0880AC84(char *area, Attack08807D4C *attack);

int func_08807D4C(Attack08807D4C *self, Actor08807D4C *actor)
{
    if (func_088378C4(actor)) {
        return 0;
    }
    if (!func_0880AC84(actor->body->area, self)) {
        return 0;
    }
    Actor08807D4C *attacker = Actor08807D4C::controller(self->owner);
    Actor08807D4C *target = Actor08807D4C::controller(actor);
    if (attacker == target) {
        return 0;
    }
    return 1;
}

}
