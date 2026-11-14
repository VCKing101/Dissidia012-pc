typedef struct CueState {
    int cue;
    char pad_04[0x54];
    void *effect;
    unsigned char enabled[2];
} CueState;

extern void func_088A6C70(void *effect);

/* Enable or disable one channel; disabling stops the effect. */
void func_08ACBEC4(CueState *self, unsigned char on, int channel)
{
    if (!on) {
        if (self->effect != 0) {
            func_088A6C70(self->effect);
        }
        self->effect = 0;
    }
    *((unsigned char *)self + 0x5C + channel) = on;
}
