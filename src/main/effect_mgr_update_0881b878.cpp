/* Effect manager update passes. */
struct Effect0881B878 {
    Effect0881B878 *next;
};

struct Actor0881B878 {
    char pad_000[0x384];
    unsigned int flags;
};

struct EffectMgr0881B878 {
    unsigned char ready;
    char pad_01[7];
    unsigned char locked;
    char pad_09[7];
    unsigned char paused;
    char pad_11[3];
    char sounds[0x48];
    char lights[1];
    char pad_5D[0x110 - 0x5D];
    Effect0881B878 *first;
};

extern "C" {

int func_08841F64(int kind);
Actor0881B878 *func_08841F84(Actor0881B878 *actor);
void func_0881D900(EffectMgr0881B878 *self);
void func_08818CB8(Effect0881B878 *e);
void func_08816868(char *sounds);
void func_08819C90(char *lights);
void func_08818D10(Effect0881B878 *e);
void func_0881D478(EffectMgr0881B878 *self);

/* Per-frame update: clears the actors' effect flag, then updates the
   effects, the sounds (unless paused) and the lights. */
int func_0881B878(EffectMgr0881B878 *self)
{
    if (!self->ready) {
        return -1;
    }
    if (!func_08841F64(0)) {
        return 0;
    }
    Actor0881B878 *a = 0;
    while ((a = func_08841F84(a)) != 0) {
        a->flags &= ~0x01000000;
    }
    if (self->locked == 1) {
        return 0;
    }
    func_0881D900(self);
    if (self->locked == 0) {
        for (Effect0881B878 *e = self->first; e != 0; e = e->next) {
            func_08818CB8(e);
        }
    }
    if (!self->paused) {
        func_08816868(self->sounds);
    }
    func_08819C90(self->lights);
    return 0;
}

/* Late update of every effect. */
int func_0881B970(EffectMgr0881B878 *self)
{
    if (!self->ready) {
        return -1;
    }
    if (!func_08841F64(0)) {
        return 0;
    }
    if (self->locked == 1) {
        return 0;
    }
    for (Effect0881B878 *e = self->first; e != 0; e = e->next) {
        func_08818D10(e);
    }
    func_0881D478(self);
    return 0;
}

}
