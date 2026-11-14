/* Scales value by every modifier whose threshold is at least the level; 1 when any applied. */
struct Rate0883ED80 {
    short threshold;
    short percent;

    static bool applies(Rate0883ED80 *r, int level) { return level <= r->threshold; }
};

struct Modifier0883ED80 {
    int field_00;
    Rate0883ED80 rate;
    int field_08;
    Modifier0883ED80 *next;
};

extern "C" {

int func_0883ED80(int *value, Modifier0883ED80 *mod, int level)
{
    int applied = 0;
    for (; mod != 0; mod = mod->next) {
        Rate0883ED80 *rate = &mod->rate;
        if (Rate0883ED80::applies(rate, level)) {
            applied = 1;
            *value = *value * rate->percent / 100;
        }
    }
    return applied;
}

}
