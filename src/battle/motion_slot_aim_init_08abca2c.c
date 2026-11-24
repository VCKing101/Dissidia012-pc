// Starts motion slot 4: aims from the slot's start point to the target, storing distance and unit direction.
typedef struct MotionSource MotionSource;
typedef struct MotionTiming { unsigned char done; float elapsed; float duration; MotionSource *source; } MotionTiming;
struct Src { int rate; char pad[0x10]; float duration; };

typedef struct MotionSlot {
    char pad_00[0x4];
    short motion;
    unsigned char flag_06;
    unsigned char flag_07;
    char start_08[0xC];
    float rate;
    char pad_18[0x18];
    char target_30[0xC];
    char dir_3c[0xC];
    float dist;
    float step;
    float duration;
    MotionTiming timing;
} MotionSlot;

extern void func_089A34C0(void *target, void *source);
extern void func_089A3528(void *dst, void *a, void *b);
extern float func_089A37DC(void *v);
extern void func_089A379C(void *dst, void *src);
extern void func_08ABD194(MotionSlot *self, float duration, MotionTiming *timing, struct Src *source);

void func_08ABCA2C(MotionSlot *self, void *target, struct Src *source, float duration)
{
    void *dir;
    self->motion = 4;
    self->rate = *(float *)source;
    self->flag_06 = 0;
    self->flag_07 = 0;
    if (duration < 0.0f) {
        duration = source->duration;
    }
    func_089A34C0(self->target_30, self->start_08);
    dir = self->dir_3c;
    func_089A3528(dir, target, self->start_08);
    self->dist = func_089A37DC(dir);
    func_089A379C(dir, dir);
    self->step = 0.0f;
    self->duration = duration;
    func_08ABD194(self, duration, &self->timing, source);
}
