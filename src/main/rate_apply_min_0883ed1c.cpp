/* Scales value by every modifier whose threshold is at most the level; 1 when any applied. */
struct Rate0883ED1C {
    short threshold;
    short percent;

    static bool applies(Rate0883ED1C *r, int level) { return r->threshold <= level; }
};

struct Modifier0883ED1C {
    int field_00;
    Rate0883ED1C rate;
    int field_08;
    Modifier0883ED1C *next;
};

extern "C" {

int func_0883ED1C(int *value, Modifier0883ED1C *mod, int level)
{
    int applied = 0;
    for (; mod != 0; mod = mod->next) {
        Rate0883ED1C *rate = &mod->rate;
        if (Rate0883ED1C::applies(rate, level)) {
            applied = 1;
            *value = *value * rate->percent / 100;
        }
    }
    return applied;
}

}
