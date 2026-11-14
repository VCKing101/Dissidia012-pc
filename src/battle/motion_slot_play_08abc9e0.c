typedef struct MotionSource {
    char pad_00[0x8];
    float rate;
} MotionSource;

typedef struct MotionSlot {
    char pad_00[0x4];
    short motion;
    unsigned char flag_06;
    unsigned char flag_07;
    char target_08[0xC];
    float rate;
} MotionSlot;

extern void func_089A34C0(void *target, int arg);

void func_08ABC9E0(MotionSlot *self, int arg, MotionSource *source)
{
    self->motion = 3;
    func_089A34C0(self->target_08, arg);
    self->rate = source->rate;
    self->flag_06 = 0;
    self->flag_07 = 0;
}
