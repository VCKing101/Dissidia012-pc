/* Applies an actor's pending damage flash and steps its fade level toward
   its target, stopping at the floor or at full. */
struct Status08833404 {
    char pad_0[5];
    unsigned char flags;
};

struct Speed08833404 {
    char pad_00[0x18];
    float scale;
};

struct Actor08833404 {
    char pad_000[0x190];
    Status08833404 status;
    char pad_196[0x2A8 - 0x196];
    float flash;
    char pad_2AC[0x370 - 0x2AC];
    float alpha;
    char pad_374[0x388 - 0x374];
    unsigned long long state;
    char pad_390[0x3B0 - 0x390];
    float level;
    float speed;
    float floor;

    static bool any(Actor08833404 *a, unsigned long long mask) { return (a->state & mask) != 0; }
    static bool held(Status08833404 *s) { return (s->flags & 8) != 0; }
    static unsigned char flash_due(Actor08833404 *a, float zero)
    {
        Status08833404 *s = &a->status;
        int r = 0;
        if (!held(s) && a->flash > zero) {
            r = 1;
        }
        return r;
    }
};

extern "C" {

extern unsigned long long D_089C7F30;
extern Speed08833404 *D_089D69E0;

void func_08834D60(Actor08833404 *self, float amount);
float func_08838300(Actor08833404 *self);

void func_08833404(Actor08833404 *self)
{
    float zero = 0.0f;
    if (Actor08833404::flash_due(self, zero)) {
        func_08834D60(self, self->flash);
        self->flash = zero;
    }
    if (self->speed != zero) {
        float rate = func_08838300(self);
        if (Actor08833404::any(self, D_089C7F30)) {
            rate = D_089D69E0->scale;
        }
        self->level += self->speed * rate;
        if (self->level <= self->floor) {
            self->level = self->floor;
            self->speed = zero;
        } else if (self->level >= 1.0f) {
            self->level = 1.0f;
            self->speed = zero;
        }
        self->alpha = self->level;
    }
}

}
