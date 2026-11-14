/* Scales value by every modifier whose threshold equals the level; 1 when any applied. */
struct Rate0883EE58 {
    short threshold;
    short percent;

    static bool applies(Rate0883EE58 *r, int level) { return r->threshold == level; }
};

struct Modifier0883EE58 {
    int field_00;
    Rate0883EE58 rate;
    int field_08;
    Modifier0883EE58 *next;
};

extern "C" {

int func_0883EE58(int *value, Modifier0883EE58 *mod, int level)
{
    int applied = 0;
    for (; mod != 0; mod = mod->next) {
        Rate0883EE58 *rate = &mod->rate;
        if (Rate0883EE58::applies(rate, level)) {
            applied = 1;
            *value = *value * rate->percent / 100;
        }
    }
    return applied;
}

}
