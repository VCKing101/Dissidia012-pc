typedef struct CueFlags {
    int cue;
    float time;
    unsigned char enabled[2];
} CueFlags;

void func_08ACF678(CueFlags *self)
{
    int i;
    self->cue = 0;
    self->time = 0.0f;
    for (i = 0; i < 2; i++) {
        *((unsigned char *)self + 0x8 + i) = 1;
    }
}
