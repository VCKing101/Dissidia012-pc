typedef struct Timer08806DB8 {
    unsigned char unknown_0000[0x20];
    float limit;
    float elapsed;
} Timer08806DB8;

float func_08838300(int clock);

/* Adds the clock's step to the elapsed time, clamped to the limit. */
void func_08806DB8(Timer08806DB8 *self, int clock) {
    float limit;
    float elapsed;
    if (self->limit <= 0.0f) {
        return;
    }
    self->elapsed = self->elapsed + func_08838300(clock);
    elapsed = self->elapsed;
    limit = self->limit;
    if (elapsed < limit) {
        limit = elapsed;
    }
    self->elapsed = limit;
}
