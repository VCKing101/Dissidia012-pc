typedef struct MotionSource {
    float rate;
    char pad_04[0x10];
    float duration;
} MotionSource;

typedef struct MotionTiming {
    unsigned char done;
    float elapsed;
    float duration;
    MotionSource *source;
} MotionTiming;

typedef struct MotionSlot {
    char pad_00[0x4];
    short motion;
    unsigned char flag_06;
    unsigned char flag_07;
    char target_08[0xC];
    float rate;
    char pad_18[0x18];
    char target_30[0xC];
    float spread;
    float duration;
    MotionTiming timing;
} MotionSlot;

extern void func_089A34C0(void *target, void *source);
extern float func_08AC3CD0(void);
extern void func_08ABD194(MotionSlot *self, float duration, MotionTiming *timing, MotionSource *source);

void func_08ABCC04(MotionSlot *self, MotionSource *source)
{
    self->motion = 6;
    self->rate = source->rate;
    self->flag_06 = 0;
    self->flag_07 = 0;
    func_089A34C0(self->target_30, self->target_08);
    self->spread = func_08AC3CD0() * 2.0f;
    self->duration = source->duration;
    func_08ABD194(self, -1.0f, &self->timing, source);
}
