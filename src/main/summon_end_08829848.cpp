/* Ends a summon: stops its sound if needed, runs the common end step and
   resets the owner's hit masks; a player owner also leaves summon mode. */
struct Pair08829848 {
    unsigned long long hit;
    unsigned long long guard;
};

struct Actor08829848 {
    char pad_000[0x448];
    Pair08829848 masks;
};

struct SoundState08829848 {
    char pad_0[3];
    unsigned char flags;

    static bool playing(SoundState08829848 *s) { return (s->flags & 0x80) != 0; }
};

struct Summon08829848 {
    char pad_00[0x30];
    Actor08829848 *owner;
    char pad_34[0x60 - 0x34];
    SoundState08829848 sound;
};

extern "C" {

extern void *D_08A62280;
extern unsigned long long D_089C78F8;

void func_08A620B0(void *mixer, int channel);
void func_0882FBFC(Summon08829848 *self);
int func_0883739C(Actor08829848 *actor);
void func_08B33CD0(Actor08829848 *actor);
void func_08B30904(Actor08829848 *actor);

void func_08829848(Summon08829848 *self)
{
    SoundState08829848 *sound = &self->sound;
    if (SoundState08829848::playing(sound)) {
        func_08A620B0(D_08A62280, 0);
    }
    func_0882FBFC(self);
    Pair08829848 *m = &self->owner->masks;
    m->hit = D_089C78F8;
    m->guard = D_089C78F8;
    if (func_0883739C(self->owner)) {
        Actor08829848 *a = self->owner;
        func_08B33CD0(a);
        func_08B30904(a);
    }
}

}
