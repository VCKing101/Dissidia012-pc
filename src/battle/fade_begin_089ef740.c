typedef struct FadeTask {
    unsigned char active;
    char pad_01[0x3];
    int state;
    void *screen;
    float elapsed;
    char pad_10[0x8];
    void *target;
} FadeTask;

extern void func_088D9C8C(void *target, float from, float to);
extern void func_088D9F00(void *screen, float speed);

/* Starts the fade on the target and the screen: -1 inactive, -2 busy, -3 nothing to fade. */
int func_089EF740(FadeTask *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->state != 1) {
        return -2;
    }
    if (self->screen == 0 || self->target == 0) {
        return -3;
    }
    func_088D9C8C(self->target, -1.0f, 1.0f);
    func_088D9F00(self->screen, 8.0f);
    self->elapsed = 0.0f;
    self->state = 3;
    return 0;
}
