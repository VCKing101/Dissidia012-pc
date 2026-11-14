/* Snaps an actor to the ground while keeping its body's motion state as
   it was. */
struct BodyState08834A40 {
    float vx, vy, vz, vw;
    float ax, ay, az, aw;
    float lx, ly, lz, lw;
    float drag;
    int flags;
};

struct Body08834A40 {
    char pad_00[0x10];
    BodyState08834A40 state;
};

struct Actor08834A40 {
    char pad_000[0x314];
    Body08834A40 *body;
};

extern "C" {

void func_08834948(Actor08834A40 *self);

void func_08834A40(Actor08834A40 *self)
{
    BodyState08834A40 saved = self->body->state;
    func_08834948(self);
    self->body->state = saved;
}

}
