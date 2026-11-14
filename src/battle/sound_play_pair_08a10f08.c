typedef struct SoundPair {
    unsigned char active;
    char pad_01[0x3];
    void *field_04;
    void *field_08;
    char pad_0C[0x4];
    void *field_10;
    char pad_14[0x4];
    void *field_18;
} SoundPair;

extern void func_088DAB7C(void *bank, void *sound);

int func_08A10F08(SoundPair *self)
{
    if (!self->active) {
        return -1;
    }
    if (self->field_04 != 0) {
        func_088DAB7C(self->field_10, self->field_04);
    }
    if (self->field_08 != 0) {
        func_088DAB7C(self->field_18, self->field_08);
    }
    return 0;
}
