typedef struct FadeTrack {
    char pad_00[0x10];
    int state;
    char pad_14[0xC];
    int field_20;
    char pad_24[0x2C];
    int field_50;
    char pad_54[0x8];
    int field_5C;
} FadeTrack;

void func_08AC1B28(FadeTrack *self)
{
    if (self->field_50 != 0) {
        return;
    }
    if (self->field_5C != 0) {
        return;
    }
    self->state = 0;
    self->field_20 = 0;
}
