typedef struct Agent4Timers {
    unsigned char unknown_0000[0xB0];
    unsigned char timer_b0[0x10];
    unsigned char timer_c0[0x10];
    unsigned char timer_d0[0x10];
} Agent4Timers;

void func_089A3A44(float value, void *dst, void *src);

void func_08837A38(Agent4Timers *self) {
    func_089A3A44(0.0f, self->timer_b0, self->timer_b0);
}

void func_08837A5C(Agent4Timers *self) {
    func_089A3A44(0.0f, self->timer_c0, self->timer_c0);
}

void func_08837A80(Agent4Timers *self) {
    func_089A3A44(0.0f, self->timer_d0, self->timer_d0);
}
