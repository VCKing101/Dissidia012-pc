typedef struct Link0884EEF4 {
    unsigned char unknown_00[0x28];
    void *target;
} Link0884EEF4;

typedef struct Body0884EEF4 {
    unsigned char unknown_00[0x34];
    Link0884EEF4 link;
} Body0884EEF4;

typedef struct Actor0884EEF4 {
    unsigned char unknown_000[0x390];
    Body0884EEF4 *body;
    unsigned char unknown_394[0x134];
    unsigned char field_4C8[0x10C];
    struct Actor0884EEF4 *partner;
} Actor0884EEF4;

typedef struct Task0884EEF4 {
    unsigned char unknown_00[8];
    Actor0884EEF4 *actor;
    unsigned char unknown_0C[6];
    short state;
} Task0884EEF4;

void func_08834DF4(Actor0884EEF4 *actor, float rate);
void func_08AD0AF8(Actor0884EEF4 *actor);
void func_088A9D7C(void *target, void *dst);

void func_0884EEF4(Task0884EEF4 *self, unsigned char on) {
    Link0884EEF4 *link;
    self->state = 2;
    if (on) {
        func_08834DF4(self->actor, 0.0f);
        func_08AD0AF8(self->actor);
        link = &self->actor->body->link;
        if (link->target != 0) {
            func_088A9D7C(link->target, self->actor->field_4C8);
        }
        link = &self->actor->partner->body->link;
        if (link->target != 0) {
            func_088A9D7C(link->target, self->actor->field_4C8);
        }
    }
}
