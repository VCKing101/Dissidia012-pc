typedef struct EffectTimer {
    char pad_00[0x30];
    void *effect;
    float remaining;
    void *unit;
} EffectTimer;

extern float func_08838300(void *unit);
extern void func_088A6AA4(void *effect);

/* Count the effect down by the unit's rate and stop it at zero. */
void func_08AC0998(EffectTimer *self)
{
    if (self->effect == 0) {
        return;
    }
    self->remaining = self->remaining - func_08838300(self->unit);
    if (self->remaining <= 0.0f) {
        func_088A6AA4(self->effect);
        self->effect = 0;
    }
}
