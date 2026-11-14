typedef struct MotionSlot {
    char pad_00[0x4];
    short motion;
    unsigned char flag_06;
    unsigned char flag_07;
    char pad_08[0xC];
    float rate;
} MotionSlot;

extern float D_08ABD5AC;

void func_08ABC88C(MotionSlot *self)
{
    self->motion = 0;
    self->rate = D_08ABD5AC;
    self->flag_06 = 0;
    self->flag_07 = 0;
}
