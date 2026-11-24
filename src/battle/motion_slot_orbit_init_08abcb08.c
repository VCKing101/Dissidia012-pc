// Starts motion slot 5: random offset on x, random radius and phase, and a rate derived from a random period.
typedef struct MotionSource MotionSource;
typedef struct MotionTiming { unsigned char done; float elapsed; float duration; MotionSource *source; } MotionTiming;

typedef struct MotionSlot {
    char pad_00[0x4];
    short motion;
    unsigned char flag_06;
    unsigned char flag_07;
    char pad_08[0xC];
    float rate;
    char pad_18[0x18];
    char target_30[4];
    float x34;
    char pad_38[4];
    float radius;
    float phase;
    float step;
    MotionTiming timing;
} MotionSlot;

extern void func_089A34C0(void *target);
extern float func_08AC3CD0(void);
extern float func_08AC3D20(float lo, float hi);
extern void func_08ABD194(MotionSlot *self, float duration, MotionTiming *timing, MotionSource *source);
extern const float D_08ABD5A8, D_08ABD5B0, D_08ABD5B4, D_08ABD5B8, D_08ABD5BC, D_08ABD5C0;

void func_08ABCB08(MotionSlot *self, int unused, MotionSource *source)
{
    float x;
    self->motion = 5;
    self->rate = 0.0f;
    self->flag_06 = 0;
    self->flag_07 = 0;
    func_089A34C0(self->target_30);
    x = self->x34;
    self->x34 = x + (D_08ABD5B0 * 2.0f * func_08AC3CD0() - D_08ABD5B0);
    self->radius = func_08AC3D20(D_08ABD5B4, D_08ABD5B8);
    self->phase = func_08AC3CD0();
    self->step = (func_08AC3D20(D_08ABD5BC, D_08ABD5C0) / 60.0f) / (D_08ABD5A8 * 2.0f * self->radius);
    func_08ABD194(self, -1.0f, &self->timing, source);
}
