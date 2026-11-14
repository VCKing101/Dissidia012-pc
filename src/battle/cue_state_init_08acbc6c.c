typedef struct CueState {
    int cue;
    float time;
    unsigned char flag_08;
    float rate;
    char pad_10[0x48];
    void *effect;
    unsigned char enabled[2];
    int index;
} CueState;

void func_08ACBC6C(CueState *self)
{
    int i;
    self->effect = 0;
    self->index = -1;
    for (i = 0; i < 2; i++) {
        *((unsigned char *)self + 0x5C + i) = 1;
    }
    self->rate = 0.0f;
    self->flag_08 = 0;
    self->time = 0.0f;
    self->cue = -1;
}
