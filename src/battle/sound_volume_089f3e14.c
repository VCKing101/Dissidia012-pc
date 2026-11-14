typedef struct SoundHolder {
    unsigned char active;
    char pad_01[0x3];
    void *sound;
} SoundHolder;

extern void func_088D9C8C(void *sound, float left, float right);

float func_089F3E14(float volume, SoundHolder *self)
{
    if (!self->active) {
        return -1.0f;
    }
    if (self->sound == 0) {
        return -2.0f;
    }
    func_088D9C8C(self->sound, volume, volume);
    return 0.0f;
}
