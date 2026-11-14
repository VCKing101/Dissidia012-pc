typedef struct TargetSet80F28 {
    unsigned char active;
    char pad_001[0x13];
    int state;
    char pad_018[0x40];
    void *target;
} TargetSet80F28;

int func_08A80F28(TargetSet80F28 *self, void *target)
{
    if (!self->active) {
        return -1;
    }
    if (!target) {
        return -2;
    }
    self->state = 3;
    self->target = target;
    return 0;
}
