typedef struct FadeJob {
    void *target;
    int pad_04;
    int state;
    float time;
    int pad_10;
    unsigned char started;
} FadeJob;

extern void func_08ADF98C(FadeJob *self, void *target);

/* Restart a running fade; state 1 first rebinds the target, and everything but state 2 moves to 3. */
void func_08ADF514(FadeJob *self)
{
    if (self->state != 0) {
        self->started = 1;
        self->time = 0.0f;
        if (self->state == 1) {
            func_08ADF98C(self, self->target);
        }
        if (self->state != 2) {
            self->state = 3;
        }
    }
}
