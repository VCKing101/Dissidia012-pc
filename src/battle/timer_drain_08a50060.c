/* In state 1 (with no motion, or motion 11), counts the timer down by the
 * frame step to zero; then runs the update. */
typedef struct DrainMotion {
    unsigned char pad_00[0xC];
    short id;
} DrainMotion;

typedef struct DrainTimer {
    int state;
    unsigned char pad_04[0x2C];
    float time;
    unsigned char pad_34[0x20];
    DrainMotion *motion;
} DrainTimer;

extern float D_089D5EAC;
extern void func_08A50244(DrainTimer *self);

void func_08A50060(DrainTimer *self)
{
    if (self->state == 1 && (self->motion == 0 || self->motion->id == 0xB)) {
        self->time -= D_089D5EAC;
        if (self->time < 0.0f) {
            self->time = 0.0f;
        }
    }
    func_08A50244(self);
}
