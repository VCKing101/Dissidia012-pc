/* Scales value by every modifier whose threshold divides the level; 1 when any applied. */
struct Rate0883EEBC {
    short threshold;
    short percent;

    static bool applies(Rate0883EEBC *r, int level) { return level % r->threshold == 0; }
};

struct Modifier0883EEBC {
    int field_00;
    Rate0883EEBC rate;
    int field_08;
    Modifier0883EEBC *next;
};

extern "C" {

int func_0883EEBC(int *value, Modifier0883EEBC *mod, int level)
{
    int applied = 0;
    for (; mod != 0; mod = mod->next) {
        Rate0883EEBC *rate = &mod->rate;
        if (Rate0883EEBC::applies(rate, level)) {
            applied = 1;
            *value = *value * rate->percent / 100;
        }
    }
    return applied;
}

}
