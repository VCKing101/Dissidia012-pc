// Adds the amount to the owner's Brave (clamped to 0..10000) unless it has the blocking status and not the exempt one.
struct Stats { char pad[0x18]; float brave; };
struct Core { char pad[0x52C]; Stats *stats; };
struct Owner { char pad[8]; Core *core; };
struct Target { int actor; };

struct Has {
    static bool is(int v) { return v != 0; }
};
struct Clamp {
    static float run(float v, float lo, float hi)
    {
        if (v < lo) {
            return lo;
        }
        if (v <= hi) {
            return v;
        }
        return hi;
    }
};

extern "C" {

extern int func_08835778(int actor);
extern int func_088BF4CC(int list, int kind);

void func_08AEC628(Owner *self, Target *t, int *amount)
{
    int list = func_08835778(t->actor);
    if (Has::is(func_088BF4CC(list, 0x53)) == 0 || Has::is(func_088BF4CC(list, 0x56)) == 1) {
        Stats *s = self->core->stats;
        s->brave = Clamp::run(s->brave + (float)*amount, 0.0f, 10000.0f);
    }
}

}
