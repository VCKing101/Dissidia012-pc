/* Effect manager: applies a parameter set to every effect with an id. */
struct EffectDef0881BAF8 {
    short id;
};

struct Effect0881BAF8 {
    Effect0881BAF8 *next;
    char pad_04[0x48 - 4];
    EffectDef0881BAF8 *def;
};

struct EffectMgr0881BAF8 {
    unsigned char ready;
    char pad_01[7];
    unsigned char locked;
    char pad_09[0x110 - 9];
    Effect0881BAF8 *first;

    static Effect0881BAF8 *next(EffectMgr0881BAF8 *m, Effect0881BAF8 *e) { return e == 0 ? m->first : e->next; }
};

extern "C" {

void func_08819624(Effect0881BAF8 *e, int mode, unsigned char a, int b, unsigned char c, int d, unsigned char f);

int func_0881BAF8(EffectMgr0881BAF8 *self, int id, unsigned char a, int b, unsigned char c, int d, unsigned char f)
{
    if (!self->ready) {
        return -1;
    }
    if (self->locked == 1) {
        return 0;
    }
    Effect0881BAF8 *e = 0;
    while ((e = EffectMgr0881BAF8::next(self, e)) != 0) {
        if (e->def->id != id) {
            continue;
        }
        func_08819624(e, 0, a, b, c, d, f);
    }
    return 0;
}

}
