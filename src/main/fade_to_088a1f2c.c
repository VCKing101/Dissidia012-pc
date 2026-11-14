/* Fades to target over duration (jumps when the duration is not positive). */
typedef struct Fade088A1F2C {
    char pad_00[0x20];
    float from;
    float to;
    float rate;
} Fade088A1F2C;

void func_088A1F2C(Fade088A1F2C *f, float target, float duration)
{
    float zero = 0.0f;
    if (!(duration <= zero)) {
        f->to = target;
        f->rate = (target - f->from) / duration;
    } else {
        f->from = target;
        f->to = target;
        f->rate = zero;
    }
}
