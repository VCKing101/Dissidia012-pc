// Adds the amount to the owner's Brave (clamped to 0..10000) unless it has the blocking status and not the exempt one.
struct Stats { char pad[0x18]; float brave; float brave2; };
struct Rec { char pad[4]; short pct; };
struct Core { char pad[0x52C]; Stats *stats; };
struct Target { Core *actor; };
struct Owner { int pad; };

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

extern int func_08835778(Core *actor);
extern Rec *func_088BF4CC(int list, int kind);

void func_08AEDB98(Owner *self, Target *t)
{
    Core *actor = t->actor;
    int list = func_08835778(actor);
    Rec *r;
    if (!Has::is((int)func_088BF4CC(list, 0x29)) && (r = func_088BF4CC(list, 0x2A)) != 0) {
        {
            short *pp = &r->pct;
            Stats *s = actor->stats;
            s->brave2 = Clamp::run(s->brave2 + (float)(*pp * 10000 / 100), 0.0f, 10000.0f);
        }
    }
}

}
