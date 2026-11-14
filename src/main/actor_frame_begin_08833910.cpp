/* Starts an actor's frame: counts the frame, keeps last frame's action and
   sets the motion speed, frozen while the actor is paused or stopped. */
struct Speed08833910 {
    char pad_00[0x18];
    float scale;
};

struct Motion08833910 {
    char data[0x50];
};

struct Actor08833910 {
    char pad_000[0x130];
    Motion08833910 motion;
    char pad_180[0x324 - 0x130 - sizeof(Motion08833910)];
    int frame;
    char pad_328[0x380 - 0x328];
    unsigned int flags;
    char pad_384[0x388 - 0x384];
    unsigned long long state;
    char pad_390[0x3BC - 0x390];
    float motion_speed;
    char pad_3C0[0x524 - 0x3C0];
    short action;
    short last_action;

    static bool any(Actor08833910 *a, unsigned long long mask) { return (a->state & mask) != 0; }
    static bool flag_set(Actor08833910 *a, unsigned int bit) { return (a->flags & bit) != 0; }
};

extern "C" {

extern unsigned long long D_089C7F30;
extern Speed08833910 *D_089D69E0;

void func_08833658(Actor08833910 *self);
float func_08838300(Actor08833910 *self);
void func_08830444(Motion08833910 *motion, float speed);

void func_08833910(Actor08833910 *self)
{
    self->frame++;
    self->last_action = self->action;
    func_08833658(self);
    Motion08833910 *m = &self->motion;
    if (!(self->flags & 1) && !Actor08833910::flag_set(self, 0x80000)) {
        if (Actor08833910::any(self, D_089C7F30)) {
            func_08830444(m, D_089D69E0->scale * self->motion_speed);
        } else {
            func_08830444(m, func_08838300(self) * self->motion_speed);
        }
    } else {
        func_08830444(m, 0.0f);
    }
}

}
