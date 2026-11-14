/* Effect manager: releases every effect. */
struct Effect0881BA2C {
    Effect0881BA2C *next;
};

struct EffectMgr0881BA2C {
    unsigned char ready;
    char pad_01[0x110 - 1];
    Effect0881BA2C *first;

    static Effect0881BA2C *next(EffectMgr0881BA2C *m, Effect0881BA2C *e) { return e == 0 ? m->first : e->next; }
};

extern "C" {

void func_08818DB0(Effect0881BA2C *e);

int func_0881BA2C(EffectMgr0881BA2C *self)
{
    if (!self->ready) {
        return -1;
    }
    Effect0881BA2C *e = 0;
    while ((e = EffectMgr0881BA2C::next(self, e)) != 0) {
        func_08818DB0(e);
    }
    return 0;
}

}
