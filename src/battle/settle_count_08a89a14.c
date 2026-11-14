typedef struct SettleCheck {
    char pad_00[0x14];
    float lowest;
    int count;
} SettleCheck;

extern float func_08A89A84(SettleCheck *self);

int func_08A89A14(SettleCheck *self)
{
    float value = func_08A89A84(self);
    int result = 0;
    if (value < self->lowest) {
        self->count = 0;
        self->lowest = value - 1.0f;
    } else {
        self->count++;
        if (self->count >= 20) {
            result = 1;
        }
    }
    return result;
}
